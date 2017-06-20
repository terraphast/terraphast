#!/bin/bash
tree=$1
data=$2
subset_file=subset
subtree_file=subtrees
triple_file=triples
./terraphy.main.py --alignment-file=$data -c > $subset_file
./terraphy.main.py --parent-tree-file=$tree --subset-file=$subset_file -d > $subtree_file
./terraphy.main.py --subtree-file=$subtree_file -t > $triple_file
./terraphy.main.py --triplet-file=$triple_file --generate-parents
