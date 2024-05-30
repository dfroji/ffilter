
#include <set>
#include <vector>
#include <string>
#include <iostream>
#include <ctime>

#include "parser.h"
#include "image.h"

int main(int argc, char* argv[]) {
   
    const std::string DEFAULT_FILTER_SIZE = "3";

    const std::string FILTER_COMMAND_KEY = "-f";
    const std::string FILTER_SIZE_COMMAND_KEY = "-fs";
    const std::string INPUT_FILE_COMMAND_KEY = "-i";
    const std::string OUTPUT_FILE_COMMAND_KEY = "-o";

    fparser::Parser parser = fparser::Parser();

    // filter parsing
    parser.add_command(
        FILTER_COMMAND_KEY,
        [FILTER_COMMAND_KEY](std::vector<std::string> arguments) {
            if (image::FILTERS.contains(arguments[0])) {
                return std::vector<std::string>{arguments[0]};
            }

            std::cout << "Error: the argument for " 
            << FILTER_COMMAND_KEY 
            << " should be one of the following:" 
            << std::endl;

            for (const auto& filter : image::FILTERS) {
                std::cout << " - " << filter << std::endl;
            }
            return std::vector<std::string>{};
        }
    );

    // filter size parsing
    parser.add_command(
        FILTER_SIZE_COMMAND_KEY,
        [FILTER_SIZE_COMMAND_KEY, DEFAULT_FILTER_SIZE](std::vector<std::string> arguments) {
            try {
                int size = std::stoi(arguments[0]);
                if (size >= 3 && size % 2 != 0) {
                    return std::vector<std::string>{std::to_string(size)};

                } else {
                    std::cout 
                    << "Error: the argument for "  
                    << FILTER_SIZE_COMMAND_KEY 
                    << " should be greater or equal to 3 and odd."
                    << std::endl
                    << "Defaulting to " 
                    << DEFAULT_FILTER_SIZE 
                    << std::endl;
                    
                    return std::vector<std::string>{};
                }

            } catch (...) {
                std::cout 
                << "Error: the argument for " 
                << FILTER_SIZE_COMMAND_KEY 
                << " should be an integer."
                << std::endl
                << "Defaulting to "
                << DEFAULT_FILTER_SIZE
                << std::endl;

                return std::vector<std::string>{};
            }
        }
    );

    // input file parsing
    parser.add_command(
        INPUT_FILE_COMMAND_KEY,
        [](std::vector<std::string> arguments) {
            return std::vector<std::string>{arguments[0]};
        }
    );

    // output file parsing
    parser.add_command(
        OUTPUT_FILE_COMMAND_KEY,
        [](std::vector<std::string> arguments) {
            return std::vector<std::string>{arguments[0]};
        }
    );

    fparser::Map parse_results = parser.parse(argc, argv);

    // exit if filepaths not passed
    if (parse_results.find(INPUT_FILE_COMMAND_KEY) == parse_results.end()
        || parse_results.find(OUTPUT_FILE_COMMAND_KEY) == parse_results.end()
        ) {
        return 1;
    }

    // use no filter if filter was not passed
    std::string filter = "";
    if (parse_results.find(FILTER_COMMAND_KEY) != parse_results.end()) {
        filter = parse_results[FILTER_COMMAND_KEY][0];
    }

    int filter_size = std::stoi(DEFAULT_FILTER_SIZE);
    if (parse_results.find(FILTER_SIZE_COMMAND_KEY) != parse_results.end()) {
        filter_size = std::stoi(parse_results[FILTER_SIZE_COMMAND_KEY][0]);
    }

    Image image = Image(filter);

    // load image
    // exit if loading image fails
    if (!image.load_image(parse_results[INPUT_FILE_COMMAND_KEY][0])) {
        return 1;    
    }

    // apply the filter
    image.filter(filter_size);

    // save image
    // exit if saving image fails
    if (!image.save_image(parse_results[OUTPUT_FILE_COMMAND_KEY][0])) {
        return 1;
    }


    return 0;
}
