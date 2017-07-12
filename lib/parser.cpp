
#include <terraces/parser.hpp>

#include <algorithm>
#include <array>
#include <stack>
#include <stdexcept>
#include <utility>
#include <vector>

#include "utils.hpp"

namespace terraces {

// non-public parsing-utilities:
namespace parsing {

enum class token_type { lparen, rparen, name, seperator, eof };

struct token {
	token_type type;
	std::string name;

	token(token_type type, std::string name = "") : type{type}, name{std::move(name)} {}
};

struct parser_state {
	index parent;
	index self;

	parser_state(index parent, index self) : parent{parent}, self{self} {}
};

using parser_stack = std::stack<parser_state, std::vector<parser_state>>;

template <typename Iterator>
token next_token(Iterator& it, Iterator end) {
	constexpr static std::array<char, 3> special_tokens{{'(', ')', ','}};
	it = utils::skip_ws(it, end);
	if (it == end) {
		return {token_type::eof};
	}
	switch (*it) {
	case '(':
		++it;
		return {token_type::lparen};
	case ')':
		++it;
		return {token_type::rparen};
	case ',':
		++it;
		return {token_type::seperator};
	}
	const auto name_begin = it;
	it = std::find_first_of(it, end, special_tokens.begin(), special_tokens.end());
	const auto name_end = utils::reverse_skip_ws(name_begin, std::find(name_begin, it, ':'));
	return {token_type::name, {name_begin, name_end}};
}

} // namespace parsing

tree_set parse_nwk(const std::string& input) {
	auto ret = tree{};
	auto names = name_map{""};
	auto indices = index_map{};

	auto stack = parsing::parser_stack{};

	auto it = input.begin();
	const auto end = input.end();

	ret.emplace_back(none, none, none);
	auto state = parsing::parser_state{none, 0};

	for (auto token = parsing::next_token(it, end); token.type != parsing::token_type::eof;
	     token = parsing::next_token(it, end)) {
		switch (token.type) {
		case parsing::token_type::lparen: {
			const auto parent = state.self;
			const auto self = ret.size();
			utils::ensure<bad_input_error>(
			        names[parent] == "",
			        "inner node names must come AFTER their children");
			stack.push(state);
			state = parsing::parser_state{parent, self};
			ret.emplace_back(parent, none, none);
			ret[parent].lchild() = self;
			names.emplace_back();
			break;
		}
		case parsing::token_type::seperator: {
			const auto parent = state.parent;
			const auto self = ret.size();
			state.self = self;
			ret.emplace_back(parent, none, none);
			auto& parent_node = ret[parent];
			utils::ensure<bad_input_error>(parent_node.rchild() == none,
			                               "input tree is not binary");
			parent_node.rchild() = self;
			names.emplace_back();
			break;
			// no need to update state as the tree is binary to
			// begin with, which means that we will now go up a level
		}
		case parsing::token_type::rparen: {
			utils::ensure<bad_input_error>(not stack.empty(), "mismatched parenthesis");
			state = stack.top();
			stack.pop();
			break;
		}
		case parsing::token_type::name: {
			if (is_leaf(ret[state.self])) {
				names[state.self] = token.name;
				indices[token.name] = state.self;
			}
			break;
		}
		case parsing::token_type::eof:
		default: { throw std::logic_error{"dafuq?"}; }
		}
	}
	if (not names.front().empty() and names.front().back() == ';') {
		names.front().pop_back();
	}
	utils::ensure<bad_input_error>(stack.empty(), "parentheses left unclosed");
	return {std::move(ret), std::move(names), std::move(indices)};
}

std::pair<bitmatrix, index> parse_bitmatrix(std::istream& input, const index_map& indices,
                                            index tree_size) {
	auto cols = index{};
	auto rows = index{}; // mostly a dummy;
	input >> rows >> cols >> std::ws;
	utils::ensure<bad_input_error>(2 * rows - 1 == tree_size, "mismatching tree-sizes");
	auto line = std::string{};
	auto suitable_root = none;
	// last column used to check if all species are present
	auto mat = bitmatrix{tree_size, cols + 1};
	auto species_per_site = std::vector<std::size_t>(cols, 0);
	while (std::getline(input, line)) {
		if (line.empty()) {
			continue;
		}
		const auto name_start = std::find(line.rbegin(), line.rend(), ' ').base();
		const auto species_name = std::string{name_start, line.end()};
		const auto species_it = indices.find(species_name);
		utils::ensure<bad_input_error>(species_it != indices.end(),
		                               std::string{"unknown species "} + species_name);
		const auto species = (*species_it).second;

		auto it = line.begin();
		auto end = name_start;
		auto all_data_available = true;
		for (auto i = index{}; i < cols; ++i) {
			it = utils::skip_ws(it, end);
			utils::ensure<bad_input_error>(it != end, "bad table in input");
			auto c = *it++;
			if (c == '1') {
				mat.set(species, i, true);
				mat.set(species, cols, true);
				++species_per_site[i];
			} else {
				all_data_available = false;
			}
		}
		utils::ensure<bad_input_error>(++it == end, "bad table in input");
		if (all_data_available and suitable_root == none) {
			suitable_root = species;
		}
	}
	for (auto name_pair : indices) {
		bool has_species = mat.get(name_pair.second, cols);
		utils::ensure<bad_input_error>(has_species,
		                               std::string{"missing species "} + name_pair.first);
	}
	auto interesting_cols = std::vector<std::size_t>();
	for (auto i = std::size_t{}; i < cols; ++i) {
		if (species_per_site[i] > 1u) {
			interesting_cols.push_back(i);
		}
	}
	return {mat.get_cols(interesting_cols), suitable_root};
}

} // namespace terraces
