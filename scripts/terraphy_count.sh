#!/bin/bash
TERRAPHY=terraphy.main.py
tree=$1
data=$2
subsets=$(tempfile)
subtrees=$(tempfile)
triples=$(tempfile)
$TERRAPHY --coverage --alignment-file=$data > $subsets
$TERRAPHY --parent-tree-file=$tree --subset-file=$subsets -d > $subtrees
$TERRAPHY --subtree-file=$subtrees -t > $triples
$TERRAPHY --triplet-file=$triples --count-parents | grep "trees on terrace" | grep -oE "[0-9]+"
rm $subtrees
rm $triples
