/*
    Copyright (C) 2015 Tomas Flouri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Contact: Tomas Flouri <Tomas.Flouri@h-its.org>,
    Heidelberg Institute for Theoretical Studies,
    Schloss-Wolfsbrunnenweg 35, D-69118 Heidelberg, Germany
*/

#include "newick-tools.h"

int opt_precision = 7;
long opt_resolve_ladder = 0;

void * xmalloc(size_t size)
{
  const size_t alignment = 16;
  void * t = NULL;
  posix_memalign(& t, alignment, size);

  if (!t) {	//TODO error handling
    printf("Unable to allocate enough memory.");
    exit(1);
  }

  return t;
}

char * xstrndup(const char * s, size_t len)
{
  char * p = (char *)xmalloc(len+1);
  strncpy(p,s,len);
  p[len] = 0;
  return p;
}

char * xstrdup(const char * s)
{
  size_t len = strlen(s);
  char * p = (char *)xmalloc(len+1);
  return strcpy(p,s);
}



/*int main (int argc, char * argv[])
{
  const char * infile = "infile.nwk";
  const char * outfile = "outfile.nwk";
  
  //read tree
  ntree_t *in_tree = ntree_parse_newick(infile);
  
  
  //write tree
  rtree_t *out_tree = ntree_to_rtree(in_tree);
  char *newick = rtree_export_newick(out_tree);
  FILE *out = fopen(outfile, "w");
  if (out == NULL) {
  	printf("Error opening file %s\n", outfile);
  	exit(1);
  }
  fprintf(out, "%s", newick);
  
  return (0);
}*/
