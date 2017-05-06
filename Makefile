# Makefile June 2011 by Alexandros Stamatakis

CC = clang
CXX = clang++

GOOGLE_LIB_SRC_ROOT = lib/googletest
GOOGLE_LIB_BIN_ROOT = $(GOOGLE_LIB_SRC_ROOT)/build
GOOGLE_LIB_PATH = $(GOOGLE_LIB_BIN_ROOT)/googlemock/gtest
GOOGLE_MAIN_LIB = $(GOOGLE_LIB_PATH)/libgtest.a

CFLAGS = -I. -O2 -fomit-frame-pointer -funroll-loops \
		 -Weverything -Wno-padded -Wno-float-equal -Wno-vla -std=c99

CXXFLAGS = -I. -I$(GOOGLE_LIB_SRC_ROOT)/googletest/include -O2 -fomit-frame-pointer -funroll-loops \
		   -Weverything -Wno-padded -Wno-float-equal -Wno-vla -std=c++14

LIBRARIES = -lm -lgmp

RM = rm -f

objs    = terraces.o input_parser.o newick-tools/newick-tools.o newick-tools/parse_ntree.o newick-tools/ntree.o newick-tools/rtree.o newick-tools/lex_ntree.o

all : terraces terraces_test

GLOBAL_DEPS = axml.h globalVariables.h

terraces : $(objs)
	$(CC) -o terraces $(objs) $(LIBRARIES) 

terraces_test : $(objs) $(GOOGLE_MAIN_LIB) test/terraces_test.o
	$(CXX) -o terraces_test test/terraces_test.o $(LIBRARIES) -L$(GOOGLE_LIB_PATH) -lgtest -lpthread

$(GOOGLE_MAIN_LIB) : 
	mkdir -p $(GOOGLE_LIB_BIN_ROOT); cd $(GOOGLE_LIB_BIN_ROOT); cmake ..; make

terraces.o : terraces.c

test/terraces_test.o : test/terraces_test.cpp

input_parser.o : input_parser.c input_parser.h

#build the part of the newick-tools program that we need to read and write newick files
newick-tools/newick-tools.o : newick-tools/newick-tools.c newick-tools/newick-tools.h

newick-tools/parse_ntree.o : newick-tools/parse_ntree.c newick-tools/newick-tools.h

newick-tools/ntree.o : newick-tools/ntree.c

newick-tools/rtree.o : newick-tools/rtree.c

newick-tools/lex_ntree.o : newick-tools/lex_ntree.c

clean : 
	$(RM) *.o test/*.o terraces newick-tools/*.o 
	$(RM) -r $(GOOGLE_LIB_BIN_ROOT)
