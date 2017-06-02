#include "input_parser.h"
#include "terraces.h"

//TODO: Not that intuitive
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf(
                "Specify .data and .nwk as first and second argument to analyze them.\n");
    }
    input_data *read_data = parse_input_data(argv[1]);
    char *read_tree = read_newk_tree(argv[2]);

    mpz_t alliumTerraceSize;
    mpz_init(alliumTerraceSize);

    FILE *empiricalTrees = fopen("empiricalTrees", "w");

    if (read_data != NULL && read_tree != NULL) {
        missingData *m = initializeMissingData(read_data->number_of_species,
                                               read_data->number_of_partitions,
                                               const_cast<const char **>(read_data->names));

        copyDataMatrix(read_data->matrix, m);
        mpz_set_ui(alliumTerraceSize, 0);

        int errorCode = terraceAnalysis(m, read_tree, TA_COUNT + TA_ENUMERATE,
                                        empiricalTrees, &alliumTerraceSize);
        if (errorCode == TERRACE_SUCCESS) {
            char *alliumTerraceSizeString = nullptr;
            alliumTerraceSizeString = mpz_get_str(alliumTerraceSizeString, 10,
                                                  alliumTerraceSize);

            printf("Empirical data set %s terrace size %s \n", argv[1],
                   alliumTerraceSizeString);

            free(alliumTerraceSizeString);
        } else {
            printf("Error %i\n", errorCode);
        }

        freeMissingData(m);
    }

    if (read_data != nullptr) {
        free_input_data(read_data);
    }

    if (read_tree != nullptr) {
        free(read_tree);
    }

    fclose(empiricalTrees);
}
