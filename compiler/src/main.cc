#include <iostream>
#include "source_reader.hh"
#include "parser.hh"
#include "compiler.hh"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " filename.ipc output.bin" << std::endl;
        return 1;
    }

    auto parsed_source = parse_source(read_source(argv[1]));
    compile(parsed_source, argv[2]);
}