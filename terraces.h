#ifndef TERRACES_H
#define TERRACES_H

#include <stdio.h>

/* 
   Error Codes
   
   the return value of our function is an error code 
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

#define TERRACE_SUCCESS                   0 
#define TERRACE_NEWICK_ERROR             -1
#define TERRACE_SPECIES_ERROR            -2 
#define TERRACE_MATRIX_ERROR             -3 
#define TERRACE_NUM_SPECIES_ERROR        -4 
#define TERRACE_NUM_PARTITIONS_ERROR     -5
#define TERRACE_THINK_ON_YOUR_OWN_ERROR  -6
#define TERRACE_OUTPUT_FILE_ERROR        -7
/* to be extended */ 

/* Argument to control output of terraceAnalysis function (ta_outspec) */

/** 
    count unrooted trees on terrace
*/
#define TA_COUNT              1

/**
   print unrooted trees on terrace to file
   @TODO: should TA_ENUMERATE automatically imply TA_COUNT?
   @TODO: Yes it should!
*/
#define TA_ENUMERATE          2

/**
   just detect if the tree is on a terrace. this should run much quicker than TA_COUNT or TA_ENUMERATE,
   because we can brake off, as soon as we have found that thera are at least two trees
   on the terrace. 
   @TODO: how the output should look like in this case?
   @TODO: return any integer in terraceSize large than 1 if the tree is on a terrace 
*/
#define TA_DETECT             4

/**
   print trees on a terrace in compressed form using some external binary tree compression tool.
   optional, does not need to be implemented, only if you want to. 
*/
#define TA_ENUMERATE_COMPRESS 8

// data type containing data to be passed to the algorithm we want to implement 

typedef struct
{
  size_t numberOfSpecies;
  size_t numberOfPartitions;
  unsigned char *missingDataMatrix;
  char **speciesNames;
} missingData; 


/**
 * Initialize missing data data type 
 *
 * @param numberOfSpecies number of species in dataset 
 *
 * @param numberOfPartitions number of partitions in dataset 
 *
 * @param speciesNames list of species names in dataset, first entry correpsonds to first row in missingDataMatrix etc. 
 * 
 * @return poitner to missing data data structure 
 */

missingData * initializeMissingData(size_t numberOfSpecies, size_t numberOfPartitions, const char **speciesNames);


/**
 * Free missing data data structure 
 * 
 * @param m pointer to missing data data structure 
 */

void freeMissingData(missingData *m);

/**
 * set entry in missing data matrix 
 * 
 * @param m pointer to missing data data structure 
 * 
 * @param speciesNumber species index 
 *
 * @param partitionNumber partition index 
 * 
 * @param value value to be set 
 */


void setDataMatrix(missingData *m, size_t speciesNumber, size_t partitionNumber, unsigned char value);


/**
 * get entry from missing data matrix 
 * 
 * @param m pointer to missing data data structure 
 * 
 * @param speciesNumber species index 
 *
 * @param partitionNumber partition index 
 * 
 * @return the value at the specified matrix position 
 */

unsigned char getDataMatrix(missingData *m, size_t speciesNumber, size_t partitionNumber);


/**
 * copy one dimensional array containing the missing data matrix to the matrix in the missing data data type
 * 
 * @param matrix one-dimensional 
 * 
 * @param m pointer to missing data data structure 
 * 
 */

void copyDataMatrix(unsigned char *matrix, missingData *m);

/**
 * terrace analysis algorithm 
 * 
 * @param m missing data data structure 
 * 
 * @param newickTreeString tree in Newick format
 * 
 * @param ta_outspec bit mask for specifying what the algorithm shall calculate 
 * 
 * @param allTreesOnTerrace file for eventually printing all trees residing on the terrace, if there is a terrace
 * 
 * @return error code 
 */

int terraceAnalysis(
		    missingData *m,
		    const char *newickTreeString,
		    const int ta_outspec,
		    FILE *allTreesOnTerrace,
		    size_t *terraceSize		   
		    );

#endif /* TERRACES_H */
