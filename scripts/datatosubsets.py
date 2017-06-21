#!/usr/bin/python3

import argparse

parser = argparse.ArgumentParser(
    description="Convert .data file to site subsets file")
parser.add_argument("input", help="The input.data file name")
parser.add_argument("output", help="The output file name")

ARGS = parser.parse_args()
SITES = []
SPECIES_COUNT = 0

with open(ARGS.input, "r") as f:
    first_line = f.readline()
    sites_count = int(first_line.split(" ")[1])
    SPECIES_COUNT = int(first_line.split()[0])
    if __debug__:
        print("Data file contains {} species with {} sites.".format(
            SPECIES_COUNT, sites_count))

    for i in range(sites_count):
        SITES.append([])

    current_species = 1
    for line in f:
        data = line.split(" ")
        del(data[-1])
        if __debug__:
            print("Encountered species with {} sites.".format(len(data)))
        for j in range(len(data)):
            if data[j] == "1":
                SITES[j].append("s{}".format(current_species))
            elif data[j] == "0":
                pass
            else:
                raise RuntimeError("Species number {} contains illegal site existence denominator ({})".format(
                    current_species, data[j]))
        current_species += 1

    for k in range(sites_count):
        assert len(SITES[k]) <= SPECIES_COUNT

with open(ARGS.output, "w") as f:
    species_overview = " ".join(["s{}".format(i)
                                 for i in range(1, SPECIES_COUNT + 1)])
    f.write(species_overview)

    for s in SITES:
        species_represenation = " ".join([element for element in s])
        f.write("\n")
        f.write(species_represenation)
