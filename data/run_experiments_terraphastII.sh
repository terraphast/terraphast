#!/bin/bash
./benchmark.py targets.txt count_out "../terraphastII/build/terraces_main %s %s > %s" > count.csv
./benchmark.py targets.txt detect_out "../terraphastII/build/terraces_main %s %s DETECT > %s" > detect.csv
./benchmark.py targets_enumerate.txt enumerate_out "../terraphastII/build/terraces_main %s %s ENUMERATE %s > /dev/null" > enumerate.csv
./benchmark.py targets_enumerate_compressed.txt enumerate_compressed_out "../terraphastII/build/terraces_main %s %s ENUMERATE_COMPRESSED %s > /dev/null" > enumerate_compressed.csv
