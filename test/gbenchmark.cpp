#include "benchmark/benchmark_api.h"

#include "terraces.h"

void BM_terraphy_analysis__count(benchmark::State& state,
                                 const char *newick_file,
                                 const char *data_file) {
    while (state.KeepRunning()) {
        input_data *read_data = parse_input_data(data_file);
        assert(read_data != nullptr);
        char *read_tree = read_newk_tree(newick_file);
        assert(read_tree != nullptr);

        missingData *m = initializeMissingData(read_data->number_of_species,
                                               read_data->number_of_partitions,
                                               const_cast<const char **>(read_data->names));
        copyDataMatrix(read_data->matrix, m);

        mpz_t terraceSize;

        mpz_init(terraceSize);
        mpz_set_ui(terraceSize, 0);

        terraceAnalysis(m, read_tree, TA_COUNT, nullptr, &terraceSize);

        freeMissingData(m);
    }
}

BENCHMARK_CAPTURE(BM_terraphy_analysis__count, Meusemann,
                  "../input/modified/Meusemann.nwk",
                  "../input/modified/Meusemann.data");

BENCHMARK_CAPTURE(BM_terraphy_analysis__count, Allium_Tiny,
                  "../input/modified/Allium_Tiny.nwk",
                  "../input/modified/Allium_Tiny.data");

BENCHMARK_CAPTURE(BM_terraphy_analysis__count, Asplenium_1,
                  "../input/modified/Asplenium.nwk.1",
                  "../input/modified/Asplenium.data.1");

BENCHMARK_CAPTURE(BM_terraphy_analysis__count, Asplenium_2,
                  "../input/modified/Asplenium.nwk.2",
                  "../input/modified/Asplenium.data.2");

BENCHMARK_CAPTURE(BM_terraphy_analysis__count, Eucalyptus_1,
                  "../input/modified/Eucalyptus.nwk.1",
                  "../input/modified/Eucalyptus.data.1");

BENCHMARK_CAPTURE(BM_terraphy_analysis__count, Eucalyptus_2,
                  "../input/modified/Eucalyptus.nwk.2",
                  "../input/modified/Eucalyptus.data.2");

BENCHMARK_CAPTURE(BM_terraphy_analysis__count, Eucalyptus_3,
                  "../input/modified/Eucalyptus.nwk.3",
                  "../input/modified/Eucalyptus.data.3");

BENCHMARK_CAPTURE(BM_terraphy_analysis__count, Euphorbia_1,
                  "../input/modified/Euphorbia.nwk.1",
                  "../input/modified/Euphorbia.data.1");

BENCHMARK_CAPTURE(BM_terraphy_analysis__count, Euphorbia_2,
                  "../input/modified/Euphorbia.nwk.1",
                  "../input/modified/Euphorbia.data.2");

BENCHMARK_MAIN()
