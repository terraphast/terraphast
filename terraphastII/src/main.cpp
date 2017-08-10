#include "input_parser.h"
#include "terraces.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Use %s <.data> <.nwk>\n", argv[0]);
        printf(" to count the number of unrooted trees on the terrace.\n");
        printf("Use %s <.data> <.nwk> DETECT\n", argv[0]);
        printf(" to see if the tree relies on a terrace or not.\n");
        printf("Use %s <.data> <.nwk> ENUMERATE_COMPRESSED <outfile>\n", argv[0]);
        printf(" to save the compressed tree to the given <outfile>.\n");
        printf("Use %s <.data> <.nwk> ENUMERATE <outfile>\n", argv[0]);
        printf(" to save all trees to the given <outfile>.\n");
        std::exit(TERRACE_THINK_ON_YOUR_OWN_ERROR);
    }

    input_data *read_data = parse_input_data(argv[1]);
    char *read_tree = read_newk_tree(argv[2]);

    int ta_outspec = TA_COUNT;
    FILE *empiricalTrees = nullptr;
    mpz_t alliumTerraceSize;
    mpz_init(alliumTerraceSize);

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

        if(argc >= 4 && strcmp(argv[3], "DETECT") == 0) {
            ta_outspec = TA_DETECT;
        } else if(argc == 5 && strcmp(argv[3], "ENUMERATE_COMPRESSED") == 0) {
            ta_outspec = TA_ENUMERATE_COMPRESS;
        } else if(argc == 5 && strcmp(argv[3], "ENUMERATE") == 0) {
            ta_outspec = TA_ENUMERATE;
        }
        if(ta_outspec == TA_ENUMERATE_COMPRESS || ta_outspec == TA_ENUMERATE) {
            empiricalTrees = fopen(argv[4], "w");
            if(empiricalTrees == nullptr) {
                printf("Error: Cannot create file: %s\n", argv[4]);
                return -1;
            }
        }

        int errorCode = terraceAnalysis(m, read_tree, ta_outspec,
                                        empiricalTrees, &alliumTerraceSize);
        if (errorCode == TERRACE_SUCCESS) {
            char *terraceSizeString = nullptr;
            terraceSizeString = mpz_get_str(terraceSizeString, 10,
                                                  alliumTerraceSize);

            if(ta_outspec == TA_DETECT) {
                if(strcmp(terraceSizeString, "1") == 0) {
                    printf("Tree is NOT on a terrace\n");
                } else {
                    printf("Tree is on a terrace\n");
                }
            } else if (ta_outspec != TA_ENUMERATE_COMPRESS) {
                puts(terraceSizeString);
            }

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
        delete[] read_tree;
    }

    if(empiricalTrees != nullptr) {
        fclose(empiricalTrees);
    }
}
