
#include <set>
#include <vector>
#include <string>
#include <iostream>

#include "parser.h"

int main(int argc, char* argv[]) {
    
    const std::set<std::string> FILTERS = {"median"};
    const std::string DEFAULT_FILTER_SIZE = "3";

    fparser::Parser parser = fparser::Parser();

    // filter parsing
    std::string f_cmnd_key = "-f";
    parser.add_command(
        f_cmnd_key,
        [FILTERS, f_cmnd_key](std::vector<std::string> arguments) {
            if (FILTERS.contains(arguments[0])) {
                return std::vector<std::string>{arguments[0]};
            }

            std::cout << "Error: the argument for " 
            << f_cmnd_key 
            << " should be one of the following:" 
            << std::endl;

            for (const auto& filter : FILTERS) {
                std::cout << " - " << filter << std::endl;
            }
            return std::vector<std::string>{};
        }
    );

    // filter size parsing
    std::string fs_cmnd_key = "-fs";
    parser.add_command(
        fs_cmnd_key,
        [fs_cmnd_key, DEFAULT_FILTER_SIZE](std::vector<std::string> arguments) {
            try {
                int size = std::stoi(arguments[0]);
                if (size >= 3 && size % 2 != 0) {
                    return std::vector<std::string>{std::to_string(size)};

                } else {
                    std::cout 
                    << "Error: the argument for "  
                    << fs_cmnd_key 
                    << " should be greater or equal to 3 and odd."
                    << std::endl
                    << "Defaulting to " 
                    << DEFAULT_FILTER_SIZE 
                    << ". " 
                    << std::endl;
                    
                    return std::vector<std::string>{};
                }

            } catch (...) {
                std::cout 
                << "Error: the argument for " 
                << fs_cmnd_key 
                << " should be an integer."
                << std::endl
                << "Defaulting to "
                << DEFAULT_FILTER_SIZE
                << ". "
                << std::endl;

                return std::vector<std::string>{};
            }
        }
    );

    fparser::Map parse_results = parser.parse(argc, argv);

    return 0;
}
