#pragma once

template <typename T>
struct TerracesAnalysisTestParameter {
    const char *newick_file;
    const char *data_file;
    const T expected_value;
    TerracesAnalysisTestParameter(const char *p_newick_file,
                                  const char *p_data_file,
                                  T p_expected_value)
            : newick_file(p_newick_file),
              data_file(p_data_file),
              expected_value(p_expected_value) {
    }
};
