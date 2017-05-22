#include "input_parser.h"
#include "terraces.h"

#include <stdlib.h>
#include <assert.h>

static void example1();
static void example2();
static void example_with_actual_datasets(int argc, char *argv[]);

//TODO: Extract test cases as unit tests - only leave the actual application skeleton
int main(int argc, char *argv[]) {
	example1();
	//example2();
	//example_with_actual_datasets(argc, argv);

	return 0;
}

/* example from the task specification */
void example1() {
	//open some output files for enumerating all trees on a terrace
	FILE *f0 = fopen("tree1", "w");
	FILE *f1 = fopen("tree2", "w");

	//Original: use raw tree as input
	//char *newickString0 = "((s1,s2),s3,(s4,s5))"; //tree T_0 from Fig 2 in the task specification
	//char *newickString1 = "((s1,s2),s4,(s3,s5))";  //tree T_1 from Fig 2 in the task sepcification

	char *newickString0 = "test/dummy_tree1.nwk"; //tree T_0 from Fig 2 in the task specification
	char *newickString1 = "test/dummy_tree2.nwk"; //tree T_1 from Fig 2 in the task sepcification

	//names of species in newick tree that corrspond to rows in the data matrix, i.e., position 0 in array speciesNames corresponds to
	//row 0 in matrix missingDataMatrix, etc.
	const char *speciesNames[] = { "s1", "s2", "s3", "s4", "s5" };

	//we are storing missing data matrices as one-dimensonial arrays:
	//rows represent the missing data for each species
	//columns the data available per partition
	const unsigned char matrix1[] = { 1, 0, //1st row, species 0 has data for partition 0 only
			1, 0, 1, 1, 0, 1, 0, 1 }, //missing data matrix from example in the task specification

			matrix2[] = { 1, 1, //1st row, species 0 has data for partitions 0 and 1
					1, 1, 1, 1, 1, 1, 1, 1 }; //no missing data, so there should not be any terraces

	//let's initialize some missing data data structures now
	missingData *example1 = initializeMissingData(5, 2, speciesNames);
	missingData *example2 = initializeMissingData(5, 2, speciesNames);

	//now copy the missing data matrices to the data structure
	copyDataMatrix(matrix1, example1);
	copyDataMatrix(matrix2, example2);

	//variables to store the terrace size
	mpz_t terraceSize0;
	mpz_t terraceSize1;

	mpz_init(terraceSize0);
	mpz_init(terraceSize1);

	mpz_set_ui(terraceSize0, 0);
	mpz_set_ui(terraceSize1, 0);

	//error code, return value of the function you shall implement
	int errorCode;

	//now let's calculate terraces for example 1
	errorCode = terraceAnalysis(example1, newickString0,
	TA_COUNT + TA_ENUMERATE, f0, &terraceSize0);
	if (errorCode == TERRACE_SUCCESS) {
		d_printf("Test 1\n");
	} else {
		d_printf("Error %i\n", errorCode);
	}

	errorCode = terraceAnalysis(example1, newickString1,
	TA_COUNT + TA_ENUMERATE, f1, &terraceSize1);
	//now let's calculate terraces for example 2
	if (errorCode == TERRACE_SUCCESS) {
		d_printf("Test 2\n");
	} else {
		d_printf("Error %i\n", errorCode);
	}

    std::cout << "terraceSize0: " << mpz_get_ui(terraceSize0) << std::endl; //Debug
	//the terraces for example input trees 0 and 1 must be of the same size and equal to 15
	assert(
			(mpz_cmp(terraceSize0, terraceSize1) == 0) && (mpz_cmp_ui(terraceSize0, 15) == 0));

	/*********************************************************************************************************************/

	// example calling a matrix with no missing data, hence there are no terraces, or the size of all terraces is 1, or
	// there are 15 unique trees.
	// call for example tree 0
	errorCode = terraceAnalysis(example2, newickString0,
	TA_COUNT + TA_ENUMERATE, f0, &terraceSize0);
	if (errorCode == TERRACE_SUCCESS) {
		d_printf("Test 3\n");
	} else {
		d_printf("Error %i\n", errorCode);
	}

	// call for example tree 1
	errorCode = terraceAnalysis(example2, newickString1,
	TA_COUNT + TA_ENUMERATE, f1, &terraceSize1);
	if (errorCode == TERRACE_SUCCESS) {
		d_printf("Test 4\n");
	} else {
		d_printf("Error %i\n", errorCode);
	}

	//terrace size for both trees must be 1
	assert(
			(mpz_cmp(terraceSize0, terraceSize1) == 0) && (mpz_cmp_ui(terraceSize0, 1) == 0));

	freeMissingData(example1);
	freeMissingData(example2);
	fclose(f0);
	fclose(f1);
}

void example2() {
	//now let's define a weird missing data matrix, there are 6 partitions and 6 species, each species
	//has data in exactly one distinct partition, what should your function return?
	// what should your function return if we change the data matrix to:
	//
	// weirdDataMatrx[] = {1,0,0,0,0,0,
	//                     0,1,0,0,0,0,
	//                     0,0,1,0,0,0,
	//                     0,0,0,1,0,0,
	//                     0,0,0,0,1,0,
	//                     0,0,0,0,0,1};
	//
	// ?

	//open an output files for enumerating all trees on a terrace
	FILE *f0 = fopen("tree1", "w");

	const char *weirdSpeciesNames[] = { "s1", "s2", "s3", "s4", "s5", "s6" };

	const unsigned char weirdDataMatrix[] = { 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0,
			0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
			0, 1 };

	char *weirdTree = "((s1,s2),(s3,s4),(s5,s6))";

	mpz_t weirdTerraceSize;

	mpz_init(weirdTerraceSize);
	mpz_set_ui(weirdTerraceSize, 0);

	//initialize missing data data structure
	missingData *weirdExample = initializeMissingData(6, 6, weirdSpeciesNames);

	copyDataMatrix(weirdDataMatrix, weirdExample);
	int errorCode = terraceAnalysis(weirdExample, weirdTree,
	TA_COUNT + TA_ENUMERATE, f0, &weirdTerraceSize);
	if (errorCode == TERRACE_SUCCESS) {
		d_printf("Weird Test\n");
	} else {
		d_printf("Error %i\n", errorCode);
	}

	char *weirdTerraceSizeString = (char*) NULL;

	weirdTerraceSizeString = mpz_get_str(weirdTerraceSizeString, 10,
			weirdTerraceSize);

	d_printf("weird terrace size: %s\n\n", weirdTerraceSizeString);

	free(weirdTerraceSizeString);

	freeMissingData(weirdExample);
	fclose(f0);
}

/* example calls with some empirical datasets that are part of this github repository */
void example_with_actual_datasets(int argc, char *argv[]) {
	if (argc != 3) {
		printf(
				"Specify .data and .nwk as first and second argument to analyze them.\n");
	}
	input_data* read_data = parse_input_data(argv[1]);
	char* read_tree = read_newk_tree(argv[2]);

	mpz_t alliumTerraceSize;
	mpz_init(alliumTerraceSize);

	FILE *empiricalTrees = fopen("empiricalTrees", "w");

	if (read_data != NULL && read_tree != NULL) {
		missingData *m = initializeMissingData(read_data->number_of_species,
				read_data->number_of_partitions,
				(const char**) read_data->names);

		copyDataMatrix(read_data->matrix, m);
		mpz_set_ui(alliumTerraceSize, 0);

		int errorCode = terraceAnalysis(m, read_tree, TA_COUNT + TA_ENUMERATE,
				empiricalTrees, &alliumTerraceSize);
		if (errorCode == TERRACE_SUCCESS) {
			char *alliumTerraceSizeString = (char*) NULL;
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

	if (read_data != NULL) {
		free_input_data(read_data);
	}

	if (read_tree != NULL) {
		free(read_tree);
	}

	fclose(empiricalTrees);
}
