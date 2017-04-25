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

/**
   Function that tells us, given a tree, and a missing data matrix as well as its dimensions, if the tree is on a terrace, 
   how many trees there are on the terrace, it also prints trees on the terrace to file, if specified and might compress them as well. 

   We might need to change the data type of variable terraceSize that is being written in this function. Why? 
   
   @return TERRACE_SUCCESS on success, or an error code (see TERRACE_*) on failure
*/

int terraceAnalysis(
		    missingData *m,
		    /**< [in]
		       struct containing missing data matrix, number of partitions, number of species, and list of species names
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

  //some debugging print outs 

  printf("read_tree = %s\n", newickTreeString);
  printf("read_names[0] = %s\n", m->speciesNames[0]);
  printf("read_matrix[0] = %hhu\n", m->missingDataMatrix[0]);

  // figure out what we are supposed to calculate  

  const bool
    countTrees = (ta_outspec & TA_COUNT) != 0,
    enumerateTrees = (ta_outspec & TA_ENUMERATE) != 0,
    treeIsOnTerrace = (ta_outspec & TA_DETECT) != 0,
    enumerateCompressedTrees = (ta_outspec & TA_ENUMERATE_COMPRESS) != 0;

  /* some basic error checking, students, please extend this, see error codes at the end of this function */
  
  assert(m->numberOfSpecies > 3 && m->numberOfPartitions > 1); 
  
  for(i = 0; i < m->numberOfSpecies; i++) 
    for(j = 0; j < m->numberOfPartitions; j++)
      if(!(getDataMatrix(m, i, j) == 0 || getDataMatrix(m, i, j) == 1))
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


/************************************************************************************************************************/

/* functions for handling the missing data data structure */

/* allocate and initialize */

missingData * initializeMissingData(size_t numberOfSpecies, size_t numberOfPartitions, const char **speciesNames)
{ 
  missingData 
    *m = (missingData *)malloc(sizeof(missingData));

  m->numberOfSpecies = numberOfSpecies;
  
  m->numberOfPartitions = numberOfPartitions; 

  //if the species names have been passed by the application programmer just set a pointer to them
  if(speciesNames != (const char **)NULL)
    {
      m->speciesNames = (char **)speciesNames; 
      m->allocatedNameArray = 0;
    }    
  //otherwise, we assume that species names are just indexes 
  else
    {
      m->allocatedNameArray = 1;
      m->speciesNames = (char **)malloc(sizeof(char *) * numberOfSpecies);
      
      size_t 
	i;

      char 
	buffer[1024];
      
      for(i = 0; i < numberOfSpecies; i++)		 	  
	{
	  sprintf(buffer, "%zu", i);	 	        

	  size_t 
	    n = strlen(buffer);

	  m->speciesNames[i] = (char *)malloc((n + 1) * sizeof(char));

	  strcpy(m->speciesNames[i], buffer);
	}
    }

  m->missingDataMatrix = (unsigned char *)malloc(sizeof(unsigned char) * numberOfSpecies * numberOfPartitions);
  
  return m;
}

/* free */ 

void freeMissingData(missingData *m)
{
  free(m->missingDataMatrix);
  
  if(m->allocatedNameArray == 1)
    {
      size_t 
	i;
      for(i = 0; i < m->numberOfSpecies; i++)
	free(m->speciesNames[i]);      
      free(m->speciesNames);
    }
  
  free(m);
}

/* set an element in the missing data matrix */ 

void setDataMatrix(missingData *m, size_t speciesNumber, size_t partitionNumber, unsigned char value)
{
  assert(speciesNumber <  m->numberOfSpecies);
  assert(speciesNumber >= 0);

  assert(partitionNumber < m->numberOfPartitions);
  assert(partitionNumber >= 0);

  assert(value == 0 || value == 1);

  m->missingDataMatrix[speciesNumber * m->numberOfPartitions + partitionNumber] = value;
}

void copyDataMatrix(unsigned char *matrix, missingData *m)
{
  memcpy((void *)m->missingDataMatrix, (void*)matrix, m->numberOfPartitions * m->numberOfSpecies);
}

/* get an element from the missing data matrix */ 

unsigned char getDataMatrix(missingData *m, size_t speciesNumber, size_t partitionNumber)
{
  assert(speciesNumber <  m->numberOfSpecies);
  assert(speciesNumber >= 0);

  assert(partitionNumber < m->numberOfPartitions);
  assert(partitionNumber >= 0);

  unsigned char 
    value = m->missingDataMatrix[speciesNumber * m->numberOfPartitions + partitionNumber];

  assert(value == 0 || value == 1);

  return value;
}



int main (int argc, char *argv[])
{
  //example from the task specification,
  //there are real-world example input datasets further down 
  char 
    *newickString0 = "((s1,s2),s3,(s4,s5));", //tree T_0 from Fig 2 in the task specification
    *newickString1 = "((s1,s2),s4,(s3,s5))";  //tree T_1 from Fig 2 in the task sepcification 


  //names of species in newick tree that corrspond to rows in the data matrix, i.e., position 0 in array speciesNames corresponds to 
  //row 0 in matrix missingDataMatrix, etc. 
  const char 
    *speciesNames[] = {"s1","s2","s3","s4", "s5"};
    
  //we are storing missing data matrices as one-dimensonial arrays: 
  //rows represent the missing data for each species 
  //columns the data available per partition 
  const unsigned char
    matrix1[] = {1,0, //1st row, species 0 has data for partition 0 only 
		 1,0,
		 1,1,
		 0,1,
		 0,1}, //missing data matrix from example in the task specification 

    matrix2[] = {1,1, //1st row, species 0 has data for partitions 0 and 1
		 1,1,
		 1,1,
		 1,1,
		 1,1}; //no missing data, so there should not be any terraces     
    
    //let's initialize some missing data data structures now 
    missingData 
      *example1 = initializeMissingData(5, 2, speciesNames),
      *example2 = initializeMissingData(5, 2, speciesNames);

    //now copy the missing data matrices to the data structure

    copyDataMatrix((unsigned char *)matrix1, example1);
    copyDataMatrix((unsigned char *)matrix2, example2);
    
    
    //open some output files for enumerating all trees on a terrace 
  FILE 
    *f0 = fopen("tree1", "w"),
    *f1 = fopen("tree2", "w");
  
  //variables to store the terrace size 
  
  size_t     
    terraceSize0,
    terraceSize1;

  //error code, return value of the function you shall implement 

  int 
    errorCode;

  //now let's calculate terraces for example 1

  if((errorCode = terraceAnalysis(example1, newickString0, TA_COUNT+TA_ENUMERATE, f0, &terraceSize0)) == TERRACE_SUCCESS)
    printf("Test 1\n");
  else
    printf("Error handling");
  
  //now let's calculate terraces for example 2 
  if((errorCode = terraceAnalysis(example1, newickString1, TA_COUNT+TA_ENUMERATE, f1, &terraceSize1)) == TERRACE_SUCCESS)
    printf("Test 2\n");
  else
    printf("Error handling");

  //the terraces for example input trees 0 and 1 must be of the same size and equal to 15 
  assert(terraceSize0 == terraceSize1 && terraceSize0 == 15);
    

  /*********************************************************************************************************************/

  // example calling a matrix with no missing data, hence there are no terraces, or the size of all terraces is 1, or 
  // there are 15 unique trees. 


  // call for example tree 0 
  if((errorCode = terraceAnalysis(example2, newickString0, TA_COUNT+TA_ENUMERATE, f0, &terraceSize0)) == TERRACE_SUCCESS)
    printf("Test 3\n");
  else
    printf("Error handling");

  // call for example tree 1
  if((errorCode = terraceAnalysis(example2, newickString1, TA_COUNT+TA_ENUMERATE, f1, &terraceSize1)) == TERRACE_SUCCESS)
    printf("Test 4\n");
  else
    printf("Error handling");

  //terrace size for both trees must be 1
  assert(terraceSize0 == terraceSize1 && terraceSize0 == 1);

  freeMissingData(example1);
  freeMissingData(example2);
  
  
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

  const char 
    *weirdSpeciesNames[] = {"s1","s2","s3","s4", "s5", "s6"};

  const unsigned char 
    weirdDataMatrix[] = {1,1,1,1,1,1, 
			 0,1,0,0,0,0, 
			 0,0,1,0,0,0,
			 0,0,0,1,0,0,
			 0,0,0,0,1,0,
			 0,0,0,0,0,1};
  
  char 
    *weirdTree = "((s1,s2),(s3,s4),(s5,s6))"; 

  size_t
    weirdTerraceSize;

  //initialize missing data data structure 
  missingData 
    *weirdExample = initializeMissingData(6, 6, weirdSpeciesNames);
  
  copyDataMatrix((unsigned char *)weirdDataMatrix, weirdExample);
  
  if((errorCode = terraceAnalysis(weirdExample, weirdTree, TA_COUNT+TA_ENUMERATE, f0, &weirdTerraceSize)) == TERRACE_SUCCESS)
    printf("Weird Test\n");
  else
    printf("Error handling");

  printf("%zu \n", weirdTerraceSize);

  freeMissingData(weirdExample);
  
  /***************************************************************************************************************************************/
  /* example calls with some empirical datasets that are part of this github repository */ 

  if(argc == 3) 
    {
      input_data* read_data = parse_input_data(argv[1]);
      char* read_tree = read_newk_tree(argv[2]);

      size_t alliumTerraceSize;
           
      FILE 
	*empiricalTrees = fopen("empiricalTrees", "w");

      if(read_data != NULL && read_tree != NULL) 
	{	  	  
	  missingData 
	    *m = initializeMissingData(read_data->number_of_species, read_data->number_of_partitions, (const char**)read_data->names);

	  copyDataMatrix(read_data->matrix, m);

	  if((errorCode = terraceAnalysis(m, read_tree, TA_COUNT + TA_ENUMERATE, empiricalTrees, &alliumTerraceSize)) == TERRACE_SUCCESS)
	    printf("Empirical data set %s terrace size %zu \n", argv[1], alliumTerraceSize);
	  else
	    printf("Error %i\n", errorCode);

	  freeMissingData(m);	  
	}

      if (read_data != NULL)
      free_input_data(read_data);   

      if (read_tree != NULL)
	free(read_tree);   

      fclose(empiricalTrees); 
    } 
  else
    printf("Specify .data and .nwk as first and second argument to analyze them.\n");
  

  fclose(f0);
  fclose(f1);

  return 0;
}
