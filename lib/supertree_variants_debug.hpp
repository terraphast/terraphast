#ifndef SUPERTREE_VARIANTS_DEBUG_HPP
#define SUPERTREE_VARIANTS_DEBUG_HPP

#include <algorithm>
#include <ostream>
#include <vector>

#include "supertree_variants.hpp"
#include "union_find_debug.hpp"
#include "utils.hpp"

namespace terraces {

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
	logging_decorator(const Callback& cb, std::ostream& output, const name_map& names)
	        : Callback{cb}, m_output{output}, m_depth{0}, m_first_iteration{}, m_names{names} {}

	void enter(const fast_index_set& leaves) {
		output() << "<itr>\n";
		++m_depth;
		output() << "<leafs val=\"{" << utils::as_comma_separated_output(leaves, m_names)
		         << "}\" />\n";
		Callback::enter(leaves);
	}

	void begin_iteration(const bipartition_iterator& bip_it, const fast_index_set& c_occ,
	                     const constraints& c) {
		output() << "<constraints val=\"{" << utils::as_comma_separated_output(c_occ, c)
		         << "}\" />\n";
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
		Callback::begin_iteration(bip_it, c_occ, c);
	}

	void step_iteration(const bipartition_iterator& bip_it) {
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
	}

	void exit(result_type result) {
		Callback::exit(result);
		output() << "<result val=\"" << result << "\" />\n";
		--m_depth;
		output() << "</itr>\n";
	}
};

} // namespace terraces

#endif // SUPERTREE_VARIANTS_DEBUG_HPP
