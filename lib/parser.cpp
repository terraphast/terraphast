
#include <terraces/parser.hpp>

#include <algorithm>
#include <array>
#include <stack>
#include <stdexcept>
#include <vector>

#include "utils.hpp"

namespace terraces {

// non-public parsing-utilities:
namespace parsing {

enum class token_type { lparen, rparen, name, seperator, eof };

struct token {
	token_type type;
	std::string name = "";
};

struct parser_state {
	index parent = none;
	index self = 0u;
};

using parser_stack = std::stack<parser_state, std::vector<parser_state>>;

template <typename Iterator>
token next_token(Iterator& it, Iterator end) {
	constexpr static auto special_tokens = std::array<char, 3>{{'(', ')', ','}};
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
	auto state = parsing::parser_state{
	        none, 0u,
	};

	for (auto token = parsing::next_token(it, end); token.type != parsing::token_type::eof;
	     token = parsing::next_token(it, end)) {
		switch (token.type) {
		case parsing::token_type::lparen: {
			const auto parent = state.self;
			const auto self = ret.size();
			stack.push(state);
			state = parsing::parser_state{parent, self};
			ret.emplace_back(parent, none, none);
			ret.at(parent).lchild() = self;
			names.emplace_back();
			break;
		}
		case parsing::token_type::seperator: {
			const auto parent = state.parent;
			const auto self = ret.size();
			state.self = self;
			ret.emplace_back(parent, none, none);
			ret.at(parent).rchild() = self;
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
			names.at(state.self) = token.name;
			indices[token.name] = state.self;
			break;
		}
		case parsing::token_type::eof:
		default: { throw std::logic_error{"dafuq?"}; }
		}
	}
	return {std::move(ret), std::move(names), std::move(indices)};
}

std::pair<bitmatrix, index> parse_bitmatrix(std::istream& input, const index_map& indices, index tree_size) {
	auto cols = index{};
	auto rows = index{}; // mostly a dummy;
	input >> rows >> cols >> std::ws;
	auto line = std::string{};
	auto mat = bitmatrix{tree_size, cols};
	auto suitable_root = none;
	while (std::getline(input, line)) {
		if (line.empty()) {
			continue;
		}
		const auto name_start = std::find(line.rbegin(), line.rend(), ' ').base();
		const auto species = indices.at({name_start, line.end()});
		auto it = line.begin();
		auto end = name_start;
		auto all_data_available = true;
		for (auto i = index{}; i < cols; ++i) {
			it = utils::skip_ws(it, end);
			utils::ensure<bad_input_error>(it != end, "bad table in input");
			auto c = *it++;
			if (c == '1') {
				mat.set(species, i, true);
			} else {
				all_data_available = false;
			}
		}
		if (all_data_available) {
			suitable_root = species;
		}
	}
	return {mat, suitable_root};
}

} // namespace terraces
