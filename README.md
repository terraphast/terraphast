Project Home {#mainpage}
==================

Two C/C++ libraries to check for and enumerate terraces in phylogenetic tree space. You will find the implementation and documentation of each library in their corresponding folders [terraphestI](terraphastI) (for Terraphest I) and [terraphestII](terraphastII/README.md) (for Terraphest II).
You can find all the used date for result validation in the folder [data](data).

-----
## Installation

### Pre-installation requirements

You will need to install the following software packages in order to compile & run both libraries:

* [CMake](https://cmake.org/install/) version 3.5 or above
* [GMP](https://gmplib.org/manual/Installing-GMP.html) or [MPIR](http://mpir.org/downloads.html)
* [Boost](http://www.boost.org/doc/libs/1_64_0/more/getting_started/index.html) if you are going to use [terraphest II](terraphastII) 

### Download & Compilation (on unix systems)

1. Click on the `Clone or Download` button of this page in order to get the source code. 
2. Open the terminal emulator of you choice and move to the subfolders [terraphastI](terraphastI) and/or [terraphastII](terraphastII) 
3. Execute the following operations:

    1. `mkdir build` 
    2. `cd build`
    3. `cmake -DCMAKE_BUILD_TYPE=Release ..`
    4. `make`
    
    After step iv. finished without any errors you should be able to find all library files and example executables within the build folders.
    
Have a look at the additional documentations for [Terraphast I](terraphastI) and [Terraphast II](terraphastII) for more details
