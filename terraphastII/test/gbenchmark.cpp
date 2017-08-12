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
                  "../input/modified/Allium.nwk",
                  "../input/modified/Allium.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Allium_Reduced,
                  "../input/modified/Allium_Reduced.nwk",
                  "../input/modified/Allium_Reduced.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Allium_Tiny,
                  "../input/modified/Allium_Tiny.nwk",
                  "../input/modified/Allium_Tiny.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Asplenium_1,
                  "../input/modified/Asplenium.nwk.1",
                  "../input/modified/Asplenium.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Asplenium_2,
                  "../input/modified/Asplenium.nwk.2",
                  "../input/modified/Asplenium.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Bouchenak,
                  "../input/modified/Bouchenak.nwk",
                  "../input/modified/Bouchenak.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Caryophyllaceae,
                  "../input/modified/Caryophyllaceae.nwk",
                  "../input/modified/Caryophyllaceae.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Eucalyptus_1,
                  "../input/modified/Eucalyptus.nwk.1",
                  "../input/modified/Eucalyptus.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Eucalyptus_2,
                  "../input/modified/Eucalyptus.nwk.2",
                  "../input/modified/Eucalyptus.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Eucalyptus_3,
                  "../input/modified/Eucalyptus.nwk.3",
                  "../input/modified/Eucalyptus.data.3")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Euphorbia_1,
                  "../input/modified/Euphorbia.nwk.1",
                  "../input/modified/Euphorbia.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Euphorbia_2,
                  "../input/modified/Euphorbia.nwk.2",
                  "../input/modified/Euphorbia.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Ficus_1,
                  "../input/modified/Ficus.nwk.1",
                  "../input/modified/Ficus.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Ficus_2,
                  "../input/modified/Ficus.nwk.2",
                  "../input/modified/Ficus.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Ficus_3,
                  "../input/modified/Ficus.nwk.3",
                  "../input/modified/Ficus.data.3")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Iris,
                  "../input/modified/Iris.nwk",
                  "../input/modified/Iris.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Meusemann,
                  "../input/modified/Meusemann.nwk",
                  "../input/modified/Meusemann.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__detect, Pyron,
                  "../input/modified/Pyron.nwk",
                  "../input/modified/Pyron.data")->Unit(benchmark::kMillisecond);

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
                  "../input/modified/Allium.nwk",
                  "../input/modified/Allium.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Allium_Reduced,
                  "../input/modified/Allium_Reduced.nwk",
                  "../input/modified/Allium_Reduced.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Allium_Tiny,
                  "../input/modified/Allium_Tiny.nwk",
                  "../input/modified/Allium_Tiny.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Asplenium_1,
                  "../input/modified/Asplenium.nwk.1",
                  "../input/modified/Asplenium.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Asplenium_2,
                  "../input/modified/Asplenium.nwk.2",
                  "../input/modified/Asplenium.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Bouchenak,
                  "../input/modified/Bouchenak.nwk",
                  "../input/modified/Bouchenak.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Caryophyllaceae,
                  "../input/modified/Caryophyllaceae.nwk",
                  "../input/modified/Caryophyllaceae.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Eucalyptus_1,
                  "../input/modified/Eucalyptus.nwk.1",
                  "../input/modified/Eucalyptus.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Eucalyptus_2,
                  "../input/modified/Eucalyptus.nwk.2",
                  "../input/modified/Eucalyptus.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Eucalyptus_3,
                  "../input/modified/Eucalyptus.nwk.3",
                  "../input/modified/Eucalyptus.data.3")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Euphorbia_1,
                  "../input/modified/Euphorbia.nwk.1",
                  "../input/modified/Euphorbia.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Euphorbia_2,
                  "../input/modified/Euphorbia.nwk.2",
                  "../input/modified/Euphorbia.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Ficus_1,
                  "../input/modified/Ficus.nwk.1",
                  "../input/modified/Ficus.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Ficus_2,
                  "../input/modified/Ficus.nwk.2",
                  "../input/modified/Ficus.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Ficus_3,
                  "../input/modified/Ficus.nwk.3",
                  "../input/modified/Ficus.data.3")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Iris,
                  "../input/modified/Iris.nwk",
                  "../input/modified/Iris.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Meusemann,
                  "../input/modified/Meusemann.nwk",
                  "../input/modified/Meusemann.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__count, Pyron,
                  "../input/modified/Pyron.nwk",
                  "../input/modified/Pyron.data")->Unit(benchmark::kMillisecond);

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
                  "../input/modified/Allium.nwk",
                  "../input/modified/Allium.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Allium_Reduced,
                  "../input/modified/Allium_Reduced.nwk",
                  "../input/modified/Allium_Reduced.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Allium_Tiny,
                  "../input/modified/Allium_Tiny.nwk",
                  "../input/modified/Allium_Tiny.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Asplenium_1,
                  "../input/modified/Asplenium.nwk.1",
                  "../input/modified/Asplenium.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Asplenium_2,
                  "../input/modified/Asplenium.nwk.2",
                  "../input/modified/Asplenium.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Bouchenak,
                  "../input/modified/Bouchenak.nwk",
                  "../input/modified/Bouchenak.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Caryophyllaceae,
                  "../input/modified/Caryophyllaceae.nwk",
                  "../input/modified/Caryophyllaceae.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Eucalyptus_1,
                  "../input/modified/Eucalyptus.nwk.1",
                  "../input/modified/Eucalyptus.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Eucalyptus_2,
                  "../input/modified/Eucalyptus.nwk.2",
                  "../input/modified/Eucalyptus.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Eucalyptus_3,
                  "../input/modified/Eucalyptus.nwk.3",
                  "../input/modified/Eucalyptus.data.3")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Euphorbia_1,
                  "../input/modified/Euphorbia.nwk.1",
                  "../input/modified/Euphorbia.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Euphorbia_2,
                  "../input/modified/Euphorbia.nwk.2",
                  "../input/modified/Euphorbia.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Ficus_1,
                  "../input/modified/Ficus.nwk.1",
                  "../input/modified/Ficus.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Ficus_2,
                  "../input/modified/Ficus.nwk.2",
                  "../input/modified/Ficus.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Ficus_3,
                  "../input/modified/Ficus.nwk.3",
                  "../input/modified/Ficus.data.3")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Iris,
                  "../input/modified/Iris.nwk",
                  "../input/modified/Iris.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Meusemann,
                  "../input/modified/Meusemann.nwk",
                  "../input/modified/Meusemann.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate_compressed, Pyron,
                  "../input/modified/Pyron.nwk",
                  "../input/modified/Pyron.data")->Unit(benchmark::kMillisecond);

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
//                  "../input/modified/Allium.nwk",
//                  "../input/modified/Allium.data")->Unit(benchmark::kMillisecond);

//BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Allium_Reduced,
//                  "../input/modified/Allium_Reduced.nwk",
//                  "../input/modified/Allium_Reduced.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Allium_Tiny,
                  "../input/modified/Allium_Tiny.nwk",
                  "../input/modified/Allium_Tiny.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Asplenium_1,
                  "../input/modified/Asplenium.nwk.1",
                  "../input/modified/Asplenium.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Asplenium_2,
                  "../input/modified/Asplenium.nwk.2",
                  "../input/modified/Asplenium.data.2")->Unit(benchmark::kMillisecond);

//BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Bouchenak,
//                  "../input/modified/Bouchenak.nwk",
//                  "../input/modified/Bouchenak.data")->Unit(benchmark::kMillisecond);

//BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Caryophyllaceae,
//                  "../input/modified/Caryophyllaceae.nwk",
//                  "../input/modified/Caryophyllaceae.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Eucalyptus_1,
                  "../input/modified/Eucalyptus.nwk.1",
                  "../input/modified/Eucalyptus.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Eucalyptus_2,
                  "../input/modified/Eucalyptus.nwk.2",
                  "../input/modified/Eucalyptus.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Eucalyptus_3,
                  "../input/modified/Eucalyptus.nwk.3",
                  "../input/modified/Eucalyptus.data.3")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Euphorbia_1,
                  "../input/modified/Euphorbia.nwk.1",
                  "../input/modified/Euphorbia.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Euphorbia_2,
                  "../input/modified/Euphorbia.nwk.2",
                  "../input/modified/Euphorbia.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Ficus_1,
                  "../input/modified/Ficus.nwk.1",
                  "../input/modified/Ficus.data.1")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Ficus_2,
                  "../input/modified/Ficus.nwk.2",
                  "../input/modified/Ficus.data.2")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Ficus_3,
                  "../input/modified/Ficus.nwk.3",
                  "../input/modified/Ficus.data.3")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Iris,
                  "../input/modified/Iris.nwk",
                  "../input/modified/Iris.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Meusemann,
                  "../input/modified/Meusemann.nwk",
                  "../input/modified/Meusemann.data")->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_terrace_analysis__enumerate, Pyron,
                  "../input/modified/Pyron.nwk",
                  "../input/modified/Pyron.data")->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN()
