This repository contains datasets for counting trees on phylogenetic terraces

### Recreating the counts
If a count file *input.csv* is given, another file *output.csv* can be created with the following command (while in the data directory)

    while IFS=, read -r tree data count; do echo "\"${tree}\",\"${data}\",$(path/to/app $tree $data)"; done < <(sed -e '1d;s/ *" *//g' input.csv) > output.csv
