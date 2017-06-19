#ifndef SUPERTREE_VARIANTS_DEBUG_HPP
#define SUPERTREE_VARIANTS_DEBUG_HPP

#include <algorithm>
#include <ostream>
#include <vector>

#include "supertree_variants.hpp"
#include "union_find_debug.hpp"
#include "utils.hpp"

namespace terraces {

template <typename Result>
struct stack_state {
	index current_bip;
	index max_bip;
	bool right;
	Result intermediate;
};

template <typename Callback>
class stack_state_callback_decorator : public Callback {
public:
	using result_type = typename Callback::result_type;

private:
	std::vector<stack_state<result_type>> m_stack;

public:
	stack_state_callback_decorator(Callback cb) : Callback{cb} {}

	void enter(const fast_index_set&, const fast_index_set&, const constraints&) {
		Callback::enter();
		m_stack.emplace_back(0, 0, false, Callback::init_result());
	}

	result_type exit(result_type result) {
		m_stack.pop_back();
		return Callback::exit(result);
	}

	void begin_iteration(const bipartition_iterator& bip_it) {
		Callback::begin_iteration(bip_it);
		m_stack.back().current_bip = bip_it.cur_bip();
		m_stack.back().max_bip = bip_it.end_bip();
	}

	void step_iteration(const bipartition_iterator& bip_it) {
		Callback::step_iteration(bip_it);
		m_stack.back().current_bip = bip_it.cur_bip();
	}

	void right_subcall() {
		Callback::right_subcall();
		m_stack.back().right = true;
	}

	void left_subcall() {
		Callback::left_subcall();
		m_stack.back().right = false;
	}

	result_type accumulate(result_type acc, result_type val) {
		auto result = Callback::accumulate(acc, val);
		m_stack.back().intermediate = result;
		return result;
	}
};

std::ostream& operator<<(std::ostream& stream, utils::named_output<constraints, name_map> output) {
	auto c = output.entry;
	auto& n = output.names;
	stream << "lca(" << n[c.left] << "," << n[c.shared] << ") < lca(" << n[c.shared] << ","
	       << n[c.right] << ")";
	return stream;
}

template <typename Callback>
class logging_decorator : public Callback {
public:
	using result_type = typename Callback::result_type;

private:
	std::ostream& m_output;
	std::size_t m_depth;
	bool m_first_iteration;
	const name_map& m_names;

	std::ostream& output() { return m_output << std::string(m_depth, '\t'); }

public:
	logging_decorator(Callback cb, std::ostream& output, const name_map& names)
	        : Callback{cb}, m_output{output}, m_depth{0}, m_first_iteration{}, m_names{names} {}

	void enter(const fast_index_set& leaves) {
		Callback::enter(leaves);
		output() << "<itr>\n";
		++m_depth;
		output() << "<leafs val=\"{" << utils::as_comma_separated_output(leaves, m_names)
		         << "}\" />\n";
	}

	void begin_iteration(const bipartition_iterator& bip_it, const fast_index_set& c_occ,
	                     const constraints& c) {
		Callback::begin_iteration(bip_it, c_occ, c);
		output() << "<constraints val=\"{"
		         << utils::as_comma_separated_output(c_occ, c, m_names) << "}\" />\n";
		output() << "<sets val=\"[";
		bool set_first = true;
		auto& leaves = bip_it.leaves();
		for (auto rep : union_find_iterable_sets{bip_it.sets()}) {
			if (not set_first) {
				m_output << ",";
			}
			m_output << "{";
			bool el_first = true;
			for (auto el : union_find_iterable_set{bip_it.sets(), rep}) {
				if (not el_first) {
					m_output << ",";
				}
				m_output << m_names[leaves.select(el)];
				el_first = false;
			}
			m_output << "}";
			set_first = false;
		}
		m_output << "]\" />\n";
		m_first_iteration = true;
	}

	void step_iteration(const bipartition_iterator& bip_it) {
		Callback::step_iteration(bip_it);
		if (not m_first_iteration) {
			--m_depth;
			output() << "</bipartition>\n";
		}
		auto subleaves = bip_it.get_current_set();
		output() << "<bipartition l=\"{"
		         << utils::as_comma_separated_output(subleaves, m_names) << "}\" r=\"{";
		subleaves.symm_difference(bip_it.leaves());
		m_output << utils::as_comma_separated_output(subleaves, m_names) << "}\" />\n";
		++m_depth;
		m_first_iteration = false;
	}

	void finish_iteration() {
		--m_depth;
		output() << "</bipartition>\n";
		Callback::finish_iteration();
	}

	result_type exit(result_type result) {
		output() << "<result val=\"" << result << "\" />\n";
		--m_depth;
		output() << "</itr>\n";
		return Callback::exit(result);
	}
};

} // namespace terraces

#endif // SUPERTREE_VARIANTS_DEBUG_HPP
