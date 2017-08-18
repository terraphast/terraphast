#include "benchmark/benchmark.h"

#include "terraces.h"

void BM_terrace_analysis__detect(benchmark::State &state,
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

        auto ret = terraceAnalysis(m, read_tree, TA_DETECT, nullptr, &terraceSize);

        assert(ret == TERRACE_SUCCESS);

        delete[](read_tree);
        mpz_clear(terraceSize);
        free_input_data(read_data);
        freeMissingData(m);
    }
}

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Allium,
                  "../../data/subsets/Allium.nwk",
                  "../../data/subsets/Allium.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Allium_Reduced,
                  "../../data/subsets/Allium_Reduced.nwk",
                  "../../data/subsets/Allium_Reduced.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Allium_Tiny,
                  "../../data/subsets/Allium_Tiny.nwk",
                  "../../data/subsets/Allium_Tiny.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Asplenium_1,
                  "../../data/subsets/Asplenium.nwk.1",
                  "../../data/subsets/Asplenium.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Asplenium_2,
                  "../../data/subsets/Asplenium.nwk.2",
                  "../../data/subsets/Asplenium.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Bouchenak,
                  "../../data/subsets/Bouchenak.nwk",
                  "../../data/subsets/Bouchenak.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Caryophyllaceae,
                  "../../data/subsets/Caryophyllaceae.nwk",
                  "../../data/subsets/Caryophyllaceae.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Eucalyptus_1,
                  "../../data/subsets/Eucalyptus.nwk.1",
                  "../../data/subsets/Eucalyptus.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Eucalyptus_2,
                  "../../data/subsets/Eucalyptus.nwk.2",
                  "../../data/subsets/Eucalyptus.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Synthetic_7t_4p,
                  "../../data/synthetic/Synthetic_7t_4p.nwk",
                  "../../data/synthetic/Synthetic_7t_4p.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Euphorbia_1,
                  "../../data/subsets/Euphorbia.nwk.1",
                  "../../data/subsets/Euphorbia.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Euphorbia_2,
                  "../../data/subsets/Euphorbia.nwk.2",
                  "../../data/subsets/Euphorbia.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Ficus_1,
                  "../../data/subsets/Ficus.nwk.1",
                  "../../data/subsets/Ficus.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Ficus_2,
                  "../../data/subsets/Ficus.nwk.2",
                  "../../data/subsets/Ficus.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Ficus_3,
                  "../../data/subsets/Ficus.nwk.3",
                  "../../data/subsets/Ficus.data.3")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Iris,
                  "../../data/subsets/Iris.nwk",
                  "../../data/subsets/Iris.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Meusemann,
                  "../../data/subsets/Meusemann.nwk",
                  "../../data/subsets/Meusemann.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Pyron,
                  "../../data/subsets/Pyron.nwk",
                  "../../data/subsets/Pyron.data")->Unit(benchmark::kMillisecond);

void BM_terrace_analysis__count(benchmark::State &state,
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

        auto ret = terraceAnalysis(m, read_tree, TA_COUNT, nullptr, &terraceSize);

        assert(ret == TERRACE_SUCCESS);

        delete[](read_tree);
        mpz_clear(terraceSize);
        free_input_data(read_data);
        freeMissingData(m);
    }
}

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Allium,
                  "../../data/subsets/Allium.nwk",
                  "../../data/subsets/Allium.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Allium_Reduced,
                  "../../data/subsets/Allium_Reduced.nwk",
                  "../../data/subsets/Allium_Reduced.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Allium_Tiny,
                  "../../data/subsets/Allium_Tiny.nwk",
                  "../../data/subsets/Allium_Tiny.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Asplenium_1,
                  "../../data/subsets/Asplenium.nwk.1",
                  "../../data/subsets/Asplenium.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Asplenium_2,
                  "../../data/subsets/Asplenium.nwk.2",
                  "../../data/subsets/Asplenium.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Bouchenak,
                  "../../data/subsets/Bouchenak.nwk",
                  "../../data/subsets/Bouchenak.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Caryophyllaceae,
                  "../../data/subsets/Caryophyllaceae.nwk",
                  "../../data/subsets/Caryophyllaceae.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Eucalyptus_1,
                  "../../data/subsets/Eucalyptus.nwk.1",
                  "../../data/subsets/Eucalyptus.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Eucalyptus_2,
                  "../../data/subsets/Eucalyptus.nwk.2",
                  "../../data/subsets/Eucalyptus.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Synthetic_7t_4p,
                  "../../data/synthetic/Synthetic_7t_4p.nwk",
                  "../../data/synthetic/Synthetic_7t_4p.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Euphorbia_1,
                  "../../data/subsets/Euphorbia.nwk.1",
                  "../../data/subsets/Euphorbia.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Euphorbia_2,
                  "../../data/subsets/Euphorbia.nwk.2",
                  "../../data/subsets/Euphorbia.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Ficus_1,
                  "../../data/subsets/Ficus.nwk.1",
                  "../../data/subsets/Ficus.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Ficus_2,
                  "../../data/subsets/Ficus.nwk.2",
                  "../../data/subsets/Ficus.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Ficus_3,
                  "../../data/subsets/Ficus.nwk.3",
                  "../../data/subsets/Ficus.data.3")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Iris,
                  "../../data/subsets/Iris.nwk",
                  "../../data/subsets/Iris.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Meusemann,
                  "../../data/subsets/Meusemann.nwk",
                  "../../data/subsets/Meusemann.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Pyron,
                  "../../data/subsets/Pyron.nwk",
                  "../../data/subsets/Pyron.data")->Unit(benchmark::kMillisecond);

void BM_terrace_analysis__enumerate_compressed(benchmark::State &state,
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

        std::FILE* tmpf = std::tmpfile();
        auto ret = terraceAnalysis(m, read_tree, TA_ENUMERATE_COMPRESS, tmpf, &terraceSize);
        assert(ret == TERRACE_SUCCESS);
        std::fclose(tmpf);

        delete[](read_tree);
        mpz_clear(terraceSize);
        free_input_data(read_data);
        freeMissingData(m);
    }
}

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Allium,
                  "../../data/subsets/Allium.nwk",
                  "../../data/subsets/Allium.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Allium_Reduced,
                  "../../data/subsets/Allium_Reduced.nwk",
                  "../../data/subsets/Allium_Reduced.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Allium_Tiny,
                  "../../data/subsets/Allium_Tiny.nwk",
                  "../../data/subsets/Allium_Tiny.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Asplenium_1,
                  "../../data/subsets/Asplenium.nwk.1",
                  "../../data/subsets/Asplenium.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Asplenium_2,
                  "../../data/subsets/Asplenium.nwk.2",
                  "../../data/subsets/Asplenium.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Bouchenak,
                  "../../data/subsets/Bouchenak.nwk",
                  "../../data/subsets/Bouchenak.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Caryophyllaceae,
                  "../../data/subsets/Caryophyllaceae.nwk",
                  "../../data/subsets/Caryophyllaceae.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Eucalyptus_1,
                  "../../data/subsets/Eucalyptus.nwk.1",
                  "../../data/subsets/Eucalyptus.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Eucalyptus_2,
                  "../../data/subsets/Eucalyptus.nwk.2",
                  "../../data/subsets/Eucalyptus.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Synthetic_7t_4p,
                  "../../data/synthetic/Synthetic_7t_4p.nwk",
                  "../../data/synthetic/Synthetic_7t_4p.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Euphorbia_1,
                  "../../data/subsets/Euphorbia.nwk.1",
                  "../../data/subsets/Euphorbia.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Euphorbia_2,
                  "../../data/subsets/Euphorbia.nwk.2",
                  "../../data/subsets/Euphorbia.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Ficus_1,
                  "../../data/subsets/Ficus.nwk.1",
                  "../../data/subsets/Ficus.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Ficus_2,
                  "../../data/subsets/Ficus.nwk.2",
                  "../../data/subsets/Ficus.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Ficus_3,
                  "../../data/subsets/Ficus.nwk.3",
                  "../../data/subsets/Ficus.data.3")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Iris,
                  "../../data/subsets/Iris.nwk",
                  "../../data/subsets/Iris.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Meusemann,
                  "../../data/subsets/Meusemann.nwk",
                  "../../data/subsets/Meusemann.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Pyron,
                  "../../data/subsets/Pyron.nwk",
                  "../../data/subsets/Pyron.data")->Unit(benchmark::kMillisecond);

void BM_terrace_analysis__enumerate(benchmark::State &state,
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

        std::FILE* tmpf = std::tmpfile();
        auto ret = terraceAnalysis(m, read_tree, TA_ENUMERATE, tmpf, &terraceSize);
        assert(ret == TERRACE_SUCCESS);
        std::fclose(tmpf);

        delete[](read_tree);
        mpz_clear(terraceSize);
        free_input_data(read_data);
        freeMissingData(m);
    }
}

//BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Allium,
//                  "../../data/subsets/Allium.nwk",
//                  "../../data/subsets/Allium.data")->Unit(benchmark::kMillisecond);

//BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Allium_Reduced,
//                  "../../data/subsets/Allium_Reduced.nwk",
//                  "../../data/subsets/Allium_Reduced.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Allium_Tiny,
                  "../../data/subsets/Allium_Tiny.nwk",
                  "../../data/subsets/Allium_Tiny.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Asplenium_1,
                  "../../data/subsets/Asplenium.nwk.1",
                  "../../data/subsets/Asplenium.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Asplenium_2,
                  "../../data/subsets/Asplenium.nwk.2",
                  "../../data/subsets/Asplenium.data.2")->Unit(benchmark::kMillisecond);

//BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Bouchenak,
//                  "../../data/subsets/Bouchenak.nwk",
//                  "../../data/subsets/Bouchenak.data")->Unit(benchmark::kMillisecond);

//BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Caryophyllaceae,
//                  "../../data/subsets/Caryophyllaceae.nwk",
//                  "../../data/subsets/Caryophyllaceae.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Eucalyptus_1,
                  "../../data/subsets/Eucalyptus.nwk.1",
                  "../../data/subsets/Eucalyptus.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Eucalyptus_2,
                  "../../data/subsets/Eucalyptus.nwk.2",
                  "../../data/subsets/Eucalyptus.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Synthetic_7t_4p,
                  "../../data/synthetic/Synthetic_7t_4p.nwk",
                  "../../data/synthetic/Synthetic_7t_4p.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Euphorbia_1,
                  "../../data/subsets/Euphorbia.nwk.1",
                  "../../data/subsets/Euphorbia.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Euphorbia_2,
                  "../../data/subsets/Euphorbia.nwk.2",
                  "../../data/subsets/Euphorbia.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Ficus_1,
                  "../../data/subsets/Ficus.nwk.1",
                  "../../data/subsets/Ficus.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Ficus_2,
                  "../../data/subsets/Ficus.nwk.2",
                  "../../data/subsets/Ficus.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Ficus_3,
                  "../../data/subsets/Ficus.nwk.3",
                  "../../data/subsets/Ficus.data.3")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Iris,
                  "../../data/subsets/Iris.nwk",
                  "../../data/subsets/Iris.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Meusemann,
                  "../../data/subsets/Meusemann.nwk",
                  "../../data/subsets/Meusemann.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Pyron,
                  "../../data/subsets/Pyron.nwk",
                  "../../data/subsets/Pyron.data")->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN()
