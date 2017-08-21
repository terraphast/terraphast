#!/usr/bin/python
import os, subprocess, re, sys


def extractDatasetInfo(data_file):
    with open(data_file, 'r') as f:
        first_line = f.readline().rstrip()
        header_info = first_line.rsplit()
        dataset_name = os.path.basename(data_file).replace(".data.",".").replace(".data","")
        is_subsampled = "yes"
        if "original" in data_file:
            is_subsampled = "no"
        print "%s & %s & %s & %s \\\\" % (dataset_name, header_info[1], header_info[0], is_subsampled)

with open(sys.argv[1]) as f:
    for line in f:
        nwk_file = line.rstrip()
        data_file = nwk_file.replace("nwk","data")
        extractDatasetInfo(data_file)