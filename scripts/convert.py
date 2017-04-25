#!/usr/bin/python3

import argparse

parser = argparse.ArgumentParser(description="Convert input_data.h file to input_data.nwk and input_data.data")
parser.add_argument("file", help="The input_data.h file name")
parser.add_argument("output_directory", help="The output directory")

args = parser.parse_args()
names = []

with open(args.file) as f:
    for line in f:
        if line.find("newickString") != -1:
            parts = line.split('"')
            content = parts[1]
            with open(args.output_directory + "/" + args.file.replace(".h", ".nwk"), "w") as o_file:
                o_file.write(content)
        elif line.find("speciesNames") != -1:
            begin, content = line.split("{")
            content, end = content.split("}")
            names = list(map(lambda x : x.strip('" '), content.split(",")))
        elif line.find("missingDataMatrix") != -1:
            content = line[line.index("{{") + 2:line.rindex("}}")]
            parts = content.split("},{")
            assert len(parts) == len(names)

            with open(args.output_directory + "/" + args.file.replace(".h", ".data"), "w") as o_file:
                o_file.write("{} {}\n".format(len(names), parts[0].count(",") + 1))
                for name, matrix in zip(names, parts):
                    o_file.write(matrix.replace(",", ""))
                    o_file.write(" ")
                    o_file.write(name)
                    o_file.write("\n")


