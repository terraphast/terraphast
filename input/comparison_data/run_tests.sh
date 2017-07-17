for tree in *.nwk*; do data=${tree%.nwk*}.data${tree#*.nwk}; echo "\"${tree}\",\"${data}\",$(../../cmake-build-release/terraces_main $data $tree)"; done > output.csv
