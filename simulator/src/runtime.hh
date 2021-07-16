#pragma once

#include <vector>
#include <string>

std::vector<uint8_t> load_binary(const std::string& filename);

class Runtime {
    public:
        Runtime(const std::vector<uint8_t>& code) : code(code) {}
        void run();

        const std::vector<uint8_t>& code;
        int32_t memory[4096];
};