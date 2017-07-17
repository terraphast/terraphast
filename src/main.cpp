#include "input_parser.h"
#include "terraces.h"

//TODO: Not that intuitive
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Specify .data and .nwk as first and second argument");
        printf(" to analyze them.\n");
        std::exit(TERRACE_THINK_ON_YOUR_OWN_ERROR);
    }
    
    input_data *read_data = parse_input_data(argv[1]);
    char *read_tree = read_newk_tree(argv[2]);

    mpz_t alliumTerraceSize;
    mpz_init(alliumTerraceSize);

    FILE *empiricalTrees = fopen("empiricalTrees", "w");

    if(read_data == NULL) {
        std::cerr << "Could not parse data file.\n";
    } else if(read_tree == NULL) {
        std::cerr << "Could not read tree file.\n";
    } else {
        missingData *m = initializeMissingData(read_data->number_of_species,
                                               read_data->number_of_partitions,
                                               const_cast<const char **>(read_data->names));

        copyDataMatrix(read_data->matrix, m);
        mpz_set_ui(alliumTerraceSize, 0);

        int errorCode = terraceAnalysis(m, read_tree, TA_COUNT + TA_ENUMERATE,
                                        empiricalTrees, &alliumTerraceSize);
        if (errorCode == TERRACE_SUCCESS) {
            char *terraceSizeString = nullptr;
            terraceSizeString = mpz_get_str(terraceSizeString, 10,
                                                  alliumTerraceSize);

            puts(terraceSizeString);

            free(terraceSizeString);
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
