This repository contains datasets for counting trees on phylogenetic terraces

### Recreating the counts
*output.csv* can be created with the following command (while in the data directory)

    for tree in *.nwk*; do data=${f%.nwk*}.data${f#*.nwk}; echo "\"${tree}\",\"${data}\",$(path/to/app $tree $data)"; done > output.csv
