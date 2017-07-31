#include "input_parser.h"
#include "terraces.h"
#include <fstream>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Use " << argv[0] << " <input-data-file> <output-nexus-file>\n";
        std::exit(TERRACE_THINK_ON_YOUR_OWN_ERROR);
    }

    input_data *read_data = parse_input_data(argv[1]);

    if (read_data == nullptr) {
        std::cerr << "Could not parse data file.\n";
    } else {
        missingData *m = initializeMissingData(read_data->number_of_species,
                                               read_data->number_of_partitions,
                                               const_cast<const char **>(read_data->names));

        copyDataMatrix(read_data->matrix, m);

        std::ofstream outfile (argv[2]);
        outfile << missing_data_to_nexus(m) << std::endl;
        outfile.close();

        freeMissingData(m);
    }

    if (read_data != nullptr) {
        free_input_data(read_data);
    }
}
