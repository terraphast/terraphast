#include "input_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "util.h"

//function to free input data

void free_input_data(input_data *d) {
    for (size_t i = 0; i < d->number_of_species; ++i) {
        free(d->names[i]);
    }

    free(d->names);
    free(d->matrix);
    free(d);
}


//read newick tree string from empirical input example files

char *read_newk_tree(const char *newk_file) {
    FILE *f = fopen(newk_file, "rb");

    if (f == NULL) {
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  //same as rewind(f);

    if (fsize < 0) {
        fclose(f);
        return NULL;
    }

    char *string = new char[static_cast<size_t>(fsize) + 1];

    if (string != NULL) {
        fread(string, static_cast<size_t>(fsize), 1, f);
        string[static_cast<size_t>(fsize)] = 0;
    }

    fclose(f);

    return string;
}


//parse input data from input example files 

input_data *parse_input_data(const char *data_file) {
    FILE *f = fopen(data_file, "rb");

    if (f == NULL) {
        return NULL;
    }

    input_data *result = new input_data;

    char input_buffer[256];

    if (result != NULL) {
        fscanf(f, "%zu %zu", &(result->number_of_species), &(result->number_of_partitions));

        result->matrix = new unsigned char[result->number_of_species * result->number_of_partitions];
        result->names = new char *[result->number_of_species];

        for (size_t s = 0; s < result->number_of_species; ++s) {
            for (size_t p = 0; p < result->number_of_partitions; ++p) {
                fscanf(f, "%1hhu", &result->matrix[s * result->number_of_partitions + p]);
            }

            // discard one (whitespace)
            fgetc(f);

            fgets(input_buffer, 256, f);
            size_t len = strlen(input_buffer);
            if (len < 255) {
                result->names[s] = new char[len];
                strncpy(result->names[s], input_buffer, len);
                // replace newline by NULL
                result->names[s][len - 1] = 0;
            } else {
                assert(0);
            }
        }
    }

    fclose(f);

    return result;
}

ntree_t *get_newk_tree(const char *nwk_file) {
    ntree_t *tree = ntree_parse_newick(nwk_file);
    fix_tree(tree); //necessary since tree seems to be corrupt
    return tree;
}

ntree_t *get_newk_tree_from_string(const char *nwk_string) {
    ntree_t *tree = ntree_parse_newick_from_string(nwk_string);
    fix_tree(tree); //necessary since tree seems to be corrupt
    return tree;
}
