/**
@file
@brief terraces framework and interface definition by Alexandros Stamatakis released under GNU GPL

Alexandros.Stamatakis@gmail.com

*/

#include <assert.h>
#include <math.h>
#include <time.h> 
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "input_parser.h"


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


/* a boolean type for C */

#define TRUE             1
#define FALSE            0

typedef int boolean;

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


int terraceAnalysis(
		    const size_t numberOfSpecies, 		    
		    const size_t numberOfPartitions, 		  
		    const unsigned char missingDataMatrix[numberOfSpecies][numberOfPartitions], 
		    const char *speciesNames[numberOfSpecies],
		    const char *newickTreeString,
		    const int ta_outspec,
		    FILE *allTreesOnTerrace,
		    size_t *terraceSize		   
		    );

/**
   Function that tells us, given a tree, and a missing data matrix as well as its dimensions, if the tree is on a terrace, 
   how many trees there are on the terrace, it also prints trees on the terrace to file, if specified and might compress them as well. 

   We might need to change the data type of variable terraceSize that is being written in this function. Why? 
   
   @return TERRACE_SUCCESS on success, or an error code (see TERRACE_*) on failure
*/

int terraceAnalysis(
		    const size_t numberOfSpecies, 
		    /**< [in]
		       number of species, i.e, number of rows in the matrix. This is denoted by n in the 
		       task specification 
		    */
		    
		    const size_t numberOfPartitions, 
		    /**< [in]
		       number of partitions (columns) in the above matrix. This is denoted by p in the task specification
		    */

		    const unsigned char missingDataMatrix[numberOfSpecies][numberOfPartitions],
		    /**< [in]
		       binary matrix with the missing data pattern, here the idea is to use one char per species, we could also 
		       use one bit quite evidently, but let's avoid messing with bits here, since the matrices will not be that big 
		    */		    

		    const char *speciesNames[numberOfSpecies],
		    /**< [in]
		       array of species names to establish how they correspond to rows in the missing data matrix. 
		       species 0 in this array corresponds to row 0 in the matrix etc. 
		    */
		    
		    const char *newickTreeString, 
		    /**< [in]
		       Unrooted strictly binary phylogenetic tree, in Newick format for which we want to find out if it is on a terrace. 
		       Denoted by T in the task specification 
		    */
		    
		    const int ta_outspec,
		    /**< [in] bit-masked integer as combination of TA_* constants to control the outputs
		    */

		    FILE *allTreesOnTerrace,
		    
		    /**< [out]
		       output file for unrooted tree enumeration. Trees should be displayed in standard unrooted Newick format, 
		       and you should print one tree 
		       per line. 
		    */

		    size_t *terraceSize

		    /**< [out] number of unrooted trees on the terrace */

		    )
{
  size_t 
    i = 0,
    j = 0;
  
  *terraceSize = 0;

  const boolean 
    countTrees = (ta_outspec & TA_COUNT) != 0,
    enumerateTrees = (ta_outspec & TA_ENUMERATE) != 0,
    treeIsOnTerrace = (ta_outspec & TA_DETECT) != 0,
    enumerateCompressedTrees = (ta_outspec & TA_ENUMERATE_COMPRESS) != 0;

  /* some basic error checking, please extend this, see error codes at the end of this function */
  
  assert(numberOfSpecies > 3 && numberOfPartitions > 1); 
  
  for(i = 0; i < numberOfSpecies; i++) 
    for(j = 0; j < numberOfPartitions; j++)
      if(!(missingDataMatrix[i][j] == 0 || missingDataMatrix[i][j] == 1))
	return TERRACE_MATRIX_ERROR;

  if((enumerateTrees || enumerateCompressedTrees) && allTreesOnTerrace == (FILE*)NULL)
    return TERRACE_OUTPUT_FILE_ERROR;

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
     


  return TERRACE_SUCCESS; 
}

int main (int argc, char *argv[])
{
  //example from the task specification,
  //we need to extend this with more real-world examples 

  char 
    *newickString0 = "((s1,s2),s3,(s4,s5));", //tree T_0 from Fig 2 in the task specification
    *newickString1 = "((s1,s2),s4,(s3,s5))";  //tree T_1 from Fig 2 in the task sepcification 


  //names of species in newick tree that corrspond to rows in the data matrix, i.e., position 0 in array speciesNames corresponds to 
  //row 0 in matrix missingDataMatrix, etc. 
  const char 
    *speciesNames[5] = {"s1","s2","s3","s4", "s5"};
    
  const unsigned char 
    missingDataMatrix[5][2] = {{1,0},{1,0},{1,1},{0,1},{0,1}}, //missing data matrix from example in the task specification 
    noMissingDataMatrix[5][2] = {{1,1},{1,1},{1,1},{1,1},{1,1}}; // no missing data, so there should not be any terraces 

  FILE 
    *f0 = fopen("tree1", "w"),
    *f1 = fopen("tree2", "w");
  
  //example from the task description 

  size_t     
    terraceSize0,
    terraceSize1;

  int 
    errorCode;

  if((errorCode = terraceAnalysis(5, 2, missingDataMatrix, speciesNames, newickString0, TA_COUNT+TA_ENUMERATE, f0, &terraceSize0)) == TERRACE_SUCCESS)
    printf("Test 1\n");
  else
    printf("Error handling");


  if((errorCode = terraceAnalysis(5, 2, missingDataMatrix, speciesNames, newickString1, TA_COUNT+TA_ENUMERATE, f1, &terraceSize1)) == TERRACE_SUCCESS)
    printf("Test 2\n");
  else
    printf("Error handling");

  assert(terraceSize0 == terraceSize1 && terraceSize0 == 15);
    
  // example calling a matrix with no missing data, hence there are no terraces, or the size of all terraces is 1, or 
  // there are 15 unique trees. 

  errorCode = terraceAnalysis(5, 2, noMissingDataMatrix, speciesNames, newickString0, TA_COUNT+TA_ENUMERATE, f0, &terraceSize0);
  errorCode = terraceAnalysis(5, 2, noMissingDataMatrix, speciesNames, newickString1, TA_COUNT+TA_ENUMERATE, f1, &terraceSize1);

  assert(terraceSize0 == terraceSize1 && terraceSize0 == 1);
  
  //now let's define a weird missing data matrix, there are 6 partitions and 6 species, each species 
  //has data in exactly one distinct partition, what should your function return? 
  // what should your function return if we change the data matrix to: 
  // weirdDataMatrx[6][6] = {{1,0,0,0,0,0}, {0,1,0,0,0,0}, {0,0,1,0,0,0}, {0,0,0,1,0,0}, {0,0,0,0,1,0}, {0,0,0,0,0,1}};
  // ?

  const char 
    *weirdSpeciesNames[6] = {"s1","s2","s3","s4", "s5", "s6"};

  const unsigned char 
    weirdDataMatrix[6][6] = {{1,1,1,1,1,1}, {0,1,0,0,0,0}, {0,0,1,0,0,0}, {0,0,0,1,0,0}, {0,0,0,0,1,0}, {0,0,0,0,0,1}};

  char 
    *weirdTree = "((s1,s2),(s3,s4),(s5,s6))"; 

  size_t
    weirdTerraceSize;

  errorCode = terraceAnalysis(6, 6, weirdDataMatrix, weirdSpeciesNames, weirdTree, TA_COUNT+TA_ENUMERATE, f0, &weirdTerraceSize);


  printf("%zu \n", weirdTerraceSize);
  //result value of terraceAnalysis? 

  // TODO: create a loop over all input files
  input_data* read_data = parse_input_data("input/Allium.data");
  char* read_tree = read_newk_tree("input/Allium.nwk");

  size_t alliumTerraceSize;

  // TODO: this creates a couple of warnings. Find out if we can silence them somehow.
  terraceAnalysis(read_data->number_of_species, read_data->number_of_partitions, read_data->matrix, read_data->names, read_tree, TA_COUNT + TA_ENUMERATE, f0, &alliumTerraceSize);

  free_input_data(read_data);
  free(read_tree);

  fclose(f0);
  fclose(f1);

  return 0;
}
