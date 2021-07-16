#include "runtime.hh"
#include "../../compiler/src/parser.hh"

#include <fstream>
#include <cstring>
#include <iostream>

std::vector<uint8_t> load_binary(const std::string& filename) {
    std::ifstream binary(filename, std::ios::binary | std::ios::in);

    return std::vector<uint8_t>(std::istreambuf_iterator<char>(binary), std::istreambuf_iterator<char>());
}

uint32_t concat_bytes(uint8_t a, uint8_t b) {
    return static_cast<uint32_t>((static_cast<uint16_t>(b) << 8) | a);
}

uint32_t concat_bytes(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    return static_cast<uint32_t>((static_cast<uint16_t>(a) << 24) | (static_cast<uint16_t>(b) << 16) | (static_cast<uint16_t>(c) << 8) | d);
}

void Runtime::run() {
    memset(&memory[0], 0, sizeof(int32_t) * 4096);

    for (uint32_t pc = 0; pc < code.size(); pc++) {
        InstructionByte ins = static_cast<InstructionByte>(code[pc]);
        // std::cout << "Current instruction: " << std::hex << static_cast<int>(ins) << std::endl;
        if (ins == InstructionByte::EXIT) {
            return;
        } else if (ins == InstructionByte::MOV) {
            memory[concat_bytes(code[pc + 1], code[pc + 2])] = concat_bytes(code[pc + 3], code[pc + 4], code[pc + 5], code[pc + 6]);
            pc += 6;
        } else if (ins == InstructionByte::ADD) {
            memory[concat_bytes(code[pc + 1], code[pc + 2])] += concat_bytes(code[pc + 3], code[pc + 4], code[pc + 5], code[pc + 6]);
            pc += 6;
        } else if (ins == InstructionByte::SUB) {
            memory[concat_bytes(code[pc + 1], code[pc + 2])] -= concat_bytes(code[pc + 3], code[pc + 4], code[pc + 5], code[pc + 6]);
            pc += 6;
        } else if (ins == InstructionByte::MUL) {
            memory[concat_bytes(code[pc + 1], code[pc + 2])] *= concat_bytes(code[pc + 3], code[pc + 4], code[pc + 5], code[pc + 6]);
            pc += 6;
        } else if (ins == InstructionByte::DIV) {
            memory[concat_bytes(code[pc + 1], code[pc + 2])] /= concat_bytes(code[pc + 3], code[pc + 4], code[pc + 5], code[pc + 6]);
            pc += 6;
        } else if (ins == InstructionByte::JMP) {
            pc = concat_bytes(code[pc + 1], code[pc + 2]) - 1;
        } else if (ins == InstructionByte::JE) {
            if (memory[concat_bytes(code[pc + 1], code[pc + 2])] == concat_bytes(code[pc + 3], code[pc + 4], code[pc + 5], code[pc + 6])) {
                pc = concat_bytes(code[pc + 7], code[pc + 8], code[pc + 9], code[pc + 10]) - 1;
            } else {
                pc += 10;
            }
        } else if (ins == InstructionByte::JNE) {
            if (memory[concat_bytes(code[pc + 1], code[pc + 2])] != concat_bytes(code[pc + 3], code[pc + 4], code[pc + 5], code[pc + 6])) {
                pc = concat_bytes(code[pc + 7], code[pc + 8], code[pc + 9], code[pc + 10]) - 1;
            } else {
                pc += 10;
            }
        } else if (ins == InstructionByte::AOUT) {
            std::cout << static_cast<unsigned int>(memory[concat_bytes(code[pc + 1], code[pc + 2])]);
            pc += 2;
        } else if (ins == InstructionByte::ASCII_AOUT) {
            std::cout << static_cast<char>(memory[concat_bytes(code[pc + 1], code[pc + 2])]);
            pc += 2;
        } else if (ins == InstructionByte::OUT) {
            std::cout << static_cast<unsigned int>(concat_bytes(code[pc + 1], code[pc + 2], code[pc + 3], code[pc + 4]));
            pc += 4;
        } else if (ins == InstructionByte::ASCII_OUT) {
            std::cout << static_cast<char>(concat_bytes(code[pc + 1], code[pc + 2], code[pc + 3], code[pc + 4]));
            pc += 4;
        } else if (ins == InstructionByte::AADD) {
            memory[concat_bytes(code[pc + 1], code[pc + 2])] += memory[concat_bytes(code[pc + 3], code[pc + 4])];
            pc += 4;
        } else if (ins == InstructionByte::ASUB) {
            memory[concat_bytes(code[pc + 1], code[pc + 2])] -= memory[concat_bytes(code[pc + 3], code[pc + 4])];
            pc += 4;
        } else if (ins == InstructionByte::AMUL) {
            memory[concat_bytes(code[pc + 1], code[pc + 2])] *= memory[concat_bytes(code[pc + 3], code[pc + 4])];
            pc += 4;
        } else if (ins == InstructionByte::ADIV) {
            memory[concat_bytes(code[pc + 1], code[pc + 2])] /= memory[concat_bytes(code[pc + 3], code[pc + 4])];
            pc += 4;
        } else if (ins == InstructionByte::AJE) {
            if (memory[concat_bytes(code[pc + 1], code[pc + 2])] == memory[concat_bytes(code[pc + 3], code[pc + 4])]) {
                pc = concat_bytes(code[pc + 5], code[pc + 6], code[pc + 7], code[pc + 8]) - 1;
            } else {
                pc += 8;
            }
        } else if (ins == InstructionByte::AJNE) {
            if (memory[concat_bytes(code[pc + 1], code[pc + 2])] != memory[concat_bytes(code[pc + 3], code[pc + 4])]) {
                pc = concat_bytes(code[pc + 5], code[pc + 6], code[pc + 7], code[pc + 8]) - 1;
            } else {
                pc += 8;
            }
        }
    }
}