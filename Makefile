# Makefile June 2011 by Alexandros Stamatakis

CC = clang  

CFLAGS = -O2 -fomit-frame-pointer -funroll-loops -Weverything -Wno-padded -Wno-float-equal -Wno-vla

LIBRARIES = -lm

RM = rm -f

objs    = terraces.o

all : terraces

GLOBAL_DEPS = axml.h globalVariables.h

terraces : $(objs)
	$(CC) -o terraces $(objs) $(LIBRARIES) 

terraces.o : terraces.c


clean : 
	$(RM) *.o terraces
