#!/bin/bash
./benchmark.py targets.txt count_out "../terraphastII/cmake-build-release/terraces_main %s %s > %s" > count.csv
#./benchmark.py targets.txt detect_out "../terraphastII/cmake-build-release/terraces_main %s %s DETECT > %s" > detect.csv
#./benchmark.py targets_enumerate.txt enumerate_out "../terraphastII/cmake-build-release/terraces_main %s %s ENUMERATE %s" > enumerate.csv
#/benchmark.py targets.txt enumerate_compressed "../terraphastII/cmake-build-release/terraces_main %s %s ENUMERATE_COMPRESSED %s" > enumerate_compressed.csv
