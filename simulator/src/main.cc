#include "runtime.hh"

#include <iostream>



int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " program.bin" << std::endl;
        return 1;
    }

    std::vector<uint8_t> program = load_binary(argv[1]);

    Runtime runtime(program);
    runtime.run();
}