#include "source_reader.hh"

#include <sstream>
#include <fstream>

std::vector<std::string> read_source(const std::string& filename) {
    std::ifstream file(filename);

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        if (line.length() == 0) continue;
        lines.push_back(std::move(line));
    }

    file.close();

    return lines;
}