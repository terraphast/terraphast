/**
 @file
 @brief terraces framework and interface definition by Alexandros Stamatakis released under GNU GPL

 Alexandros.Stamatakis@gmail.com

 */

#include "input_parser.h"
#include "terraces.h"

#include <assert.h>
#include <math.h>
#include <time.h> 
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <gmp.h>

#ifndef DEBUG
static void d_print_tree_rec(const ntree_t* tree, int depth) {
	for (int j = 0; j < depth * 4; j++) {
		printf(" ");
	}
	printf("Label: %s\n", tree->label);
	for (int i = 0; i < tree->children_count; i++) {
		d_print_tree_rec(tree->children[i], depth + 1);
	}
}

void d_print_tree(const ntree_t* tree) {
	d_printf("Dump Tree:\n");
	d_print_tree_rec(tree, 1);
}
#endif /* DEBUG */

int terraceAnalysis(missingData *m, const char *newickTreeString,
		const int ta_outspec, FILE *allTreesOnTerrace, mpz_t *terraceSize) {
	size_t i = 0;
	size_t j = 0;

	mpz_set_ui(*terraceSize, 0);

	//some debugging print outs

	d_printf("read_tree = %s\n", newickTreeString);
	d_printf("read_names[0] = %s\n", m->speciesNames[0]);
	d_printf("read_matrix[0] = %hhu\n", m->missingDataMatrix[0]);

	// figure out what we are supposed to calculate

	const bool countTrees = (ta_outspec & TA_COUNT) != 0;
	const bool enumerateTrees = (ta_outspec & TA_ENUMERATE) != 0;
	const bool treeIsOnTerrace = (ta_outspec & TA_DETECT) != 0;
	const bool enumerateCompressedTrees = (ta_outspec & TA_ENUMERATE_COMPRESS)
			!= 0;

	/* some basic error checking, students, please extend this, see error codes at the end of this function */

	assert(m->numberOfSpecies > 3 && m->numberOfPartitions > 1);

	for (i = 0; i < m->numberOfSpecies; i++) {
		for (j = 0; j < m->numberOfPartitions; j++) {
			if (!(getDataMatrix(m, i, j) == 0 || getDataMatrix(m, i, j) == 1)) {
				return TERRACE_MATRIX_ERROR;
			}
		}
	}

	if ((enumerateTrees || enumerateCompressedTrees)
			&& allTreesOnTerrace == (FILE*) NULL) {
		return TERRACE_OUTPUT_FILE_ERROR;
	}

	ntree_t *tree = get_newk_tree(newickTreeString);

	assert(tree != NULL);

	d_print_tree(tree);

	/* e.g., include an error check to make sure the Newick tree you have parsed contains as many species as indicated by numberOfSpecies */

	/*
	 the function shall write to variable terraceSize
	 1. the number of UNROOTED trees on the terrace
	 2. or just some value > 1 in terraceSize, if we only wanted to know if the tree is on a terrace
	 */

	/*
	 the return value is an error code
	 we will define these together as the project proceeds, e.g.
	 0:  succesfull completion
	 -1: problem parsing Newick file
	 -2: #species in Newick file does not correspond to number of species in data matrix
	 -3: entries in data matrix not either 0 or 1
	 -4: less than 4 spcies in input tree
	 -5: only one partition in data matrix
	 -6: reserved for something you must think about anyway (tree can't be rooted)
	 -7: no output file specified
	 */

	ntree_destroy(tree);
	return TERRACE_SUCCESS;
}

/************************************************************************************************************************/

/* functions for handling the missing data data structure */

/* allocate and initialize */

missingData * initializeMissingData(size_t numberOfSpecies,
		size_t numberOfPartitions, const char **speciesNames) {
	missingData *m = (missingData *) malloc(sizeof(missingData));

	m->numberOfSpecies = numberOfSpecies;

	m->numberOfPartitions = numberOfPartitions;

	//if the species names have been passed by the application programmer just set a pointer to them
	if (speciesNames != (const char **) NULL) {
		m->speciesNames = (char **) speciesNames;
		m->allocatedNameArray = 0;
	}
	//otherwise, we assume that species names are just indexes
	else {
		m->allocatedNameArray = 1;
		m->speciesNames = (char **) malloc(sizeof(char *) * numberOfSpecies);

		size_t i;

		char buffer[1024];

		for (i = 0; i < numberOfSpecies; i++) {
			sprintf(buffer, "%zu", i);

			size_t n = strlen(buffer);

			m->speciesNames[i] = (char *) malloc((n + 1) * sizeof(char));

			strcpy(m->speciesNames[i], buffer);
		}
	}

	m->missingDataMatrix = (unsigned char *) malloc(
			sizeof(unsigned char) * numberOfSpecies * numberOfPartitions);

	return m;
}

/* free */

void freeMissingData(missingData *m) {
	free(m->missingDataMatrix);

	if (m->allocatedNameArray == 1) {
		size_t i;
		for (i = 0; i < m->numberOfSpecies; i++)
			free(m->speciesNames[i]);
		free(m->speciesNames);
	}

	free(m);
}

/* set an element in the missing data matrix */

void setDataMatrix(missingData *m, size_t speciesNumber, size_t partitionNumber,
		unsigned char value) {
	assert(speciesNumber < m->numberOfSpecies);
	assert(speciesNumber >= 0);

	assert(partitionNumber < m->numberOfPartitions);
	assert(partitionNumber >= 0);

	assert(value == 0 || value == 1);

	m->missingDataMatrix[speciesNumber * m->numberOfPartitions + partitionNumber] =
			value;
}

void copyDataMatrix(const unsigned char *matrix, missingData *m) {
	memcpy((void *) m->missingDataMatrix, (const void*) matrix,
			m->numberOfPartitions * m->numberOfSpecies);
}

/* get an element from the missing data matrix */

unsigned char getDataMatrix(missingData *m, size_t speciesNumber,
		size_t partitionNumber) {
	assert(speciesNumber < m->numberOfSpecies);
	assert(speciesNumber >= 0);

	assert(partitionNumber < m->numberOfPartitions);
	assert(partitionNumber >= 0);

	unsigned char value = m->missingDataMatrix[speciesNumber
			* m->numberOfPartitions + partitionNumber];

	assert(value == 0 || value == 1);

	return value;
}
