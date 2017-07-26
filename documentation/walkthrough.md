## A Short Guide to the Codebase

This is a short and rather shallow guide to the codebase. Starting from main.cpp, you'll see the crucial steps and data structures involved in solving the terrace detection and enumeration problem.

### 0) Interpreting the CLI Arguments
If two file paths are provided, it is assumed that the first path points to the tree file (.nwk) and the second path points to the data file (.data).

If one file path Y/X is provided, it is assumed that the files X.nwk and X.data are present in the directory Y.

### 1) Parsing the Input Files

This is done by calling terraces::parse_nwk and terraces::parse_bitmatrix. Both throw exceptions, if the input files are not in the right format. If the .data file does not contain a species that possesses all gene sites, this is denoted by a terraces::none value in the std::pair returned by terraces::parse_bitmatrix. In this case, the current course of action is to exit with error code 1.

### 2) Rerooting the Input Tree

GO ON AFTER A NICE CUP OF TEA