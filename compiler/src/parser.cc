#include "parser.hh"

#include <sstream>
#include <string>
#include <stdexcept>
#include <algorithm>

#define get_address() int32_t address = hex_to_int(tokens[1]); if (address < 0 || address > 4096) throw std::invalid_argument("Address outside the valid range");

std::vector<std::string> tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    tokens.reserve(3);
    
    std::istringstream iss(line);
    std::string token;

    while (iss >> token) {
        tokens.push_back(std::move(token));
    }

    return tokens;
}

int32_t hex_to_int(const std::string& hex) {
    return std::stoi(hex, nullptr, 16);
}

parsed_t parse_source(const std::vector<std::string>& source) {
    std::vector<Instruction> parsed;
    std::vector<std::pair<std::string, uint32_t>> labels;

    for (int line = 0; line < source.size(); line++) {
        std::vector<std::string> tokens = tokenize(source[line]);
        if (tokens[0] == "exit") {
            parsed.push_back(Instruction(InstructionByte::EXIT));
        } else if (tokens[0] == "mov") {
            // mov addr value
            get_address();
            int32_t value = std::stoi(tokens[2]);
            parsed.push_back(Instruction(InstructionByte::MOV, {address, value}));
        } else if (tokens[0] == "add") {
            // add addr value
            get_address();
            int32_t value = std::stoi(tokens[2]);
            parsed.push_back(Instruction(InstructionByte::ADD, {address, value}));
        } else if (tokens[0] == "sub") {
            // sub addr value
            get_address();
            int32_t value = std::stoi(tokens[2]);
            parsed.push_back(Instruction(InstructionByte::SUB, {address, value}));
        } else if (tokens[0] == "mul") {
            // mul addr value
            get_address();
            int32_t value = std::stoi(tokens[2]);
            parsed.push_back(Instruction(InstructionByte::MUL, {address, value}));
        } else if (tokens[0] == "div") {
            // div addr value
            get_address();
            int32_t value = std::stoi(tokens[2]);
            parsed.push_back(Instruction(InstructionByte::DIV, {address, value}));
        } else if (tokens[0] == "je") {
            // je addr value instruction
            get_address();
            int32_t value = std::stoi(tokens[2]);
            parsed.push_back(Instruction(InstructionByte::JE, {address, value}, tokens[3]));
        } else if (tokens[0] == "jne") {
            // jne addr value instruction
            get_address();
            int32_t value = std::stoi(tokens[2]);
            parsed.push_back(Instruction(InstructionByte::JNE, {address, value}, tokens[3]));
        } else if (tokens[0] == "jmp") {
            // jmp instruction
            parsed.push_back(Instruction(InstructionByte::JMP, {}, tokens[1]));
        } else if (tokens[0] == "aout") {
            // aout addr
            get_address();
            parsed.push_back(Instruction(InstructionByte::AOUT, {address}));
        } else if (tokens[0] == "ascii-aout") {
            // ascii-aout addr
            get_address();
            parsed.push_back(Instruction(InstructionByte::ASCII_AOUT, {address}));
        } else if (tokens[0] == "out") {
            // out value
            int32_t value = std::stoi(tokens[1]);
            parsed.push_back(Instruction(InstructionByte::OUT, {value}));
        } else if (tokens[0] == "ascii-out") {
            // ascii-out
            int32_t value = std::stoi(tokens[1]);
            parsed.push_back(Instruction(InstructionByte::ASCII_OUT, {value}));
        } else if (tokens[0] == "print" || tokens[0] == "println") {
            for (int32_t i = 1; i < tokens.size(); i++) {
                for (int32_t c = 0; c < tokens[i].length(); c++) {
                    parsed.push_back(Instruction(InstructionByte::ASCII_OUT, {static_cast<int32_t>(tokens[i][c])}));
                }
                if (i != tokens.size() - 1) {
                    parsed.push_back(Instruction(InstructionByte::ASCII_OUT, {static_cast<int32_t>(' ')}));
                }
            }
            if (tokens[0] == "println") {
                parsed.push_back(Instruction(InstructionByte::ASCII_OUT, {static_cast<int32_t>('\n')}));
            }
        } else if (tokens[0] == "label") {
            std::pair<std::string, uint32_t> label = {tokens[1], line};
            if (std::find(labels.begin(), labels.end(), label) == labels.end()) {
                labels.push_back(label);
            } else {
                throw std::logic_error("Multiple labels with the same name");
            }
        } else if (tokens[0] == "aadd") {
            int32_t address_a = hex_to_int(tokens[1]);
            if (address_a < 0 || address_a > 4096) throw std::invalid_argument("Address outside the valid range");
            int32_t address_b = hex_to_int(tokens[2]);
            if (address_b < 0 || address_b > 4096) throw std::invalid_argument("Address outside the valid range");

            parsed.push_back(Instruction(InstructionByte::AADD, {address_a, address_b}));
        } else if (tokens[0] == "asub") {
            int32_t address_a = hex_to_int(tokens[1]);
            if (address_a < 0 || address_a > 4096) throw std::invalid_argument("Address outside the valid range");
            int32_t address_b = hex_to_int(tokens[2]);
            if (address_b < 0 || address_b > 4096) throw std::invalid_argument("Address outside the valid range");

            parsed.push_back(Instruction(InstructionByte::ASUB, {address_a, address_b}));
        } else if (tokens[0] == "amul") {
            int32_t address_a = hex_to_int(tokens[1]);
            if (address_a < 0 || address_a > 4096) throw std::invalid_argument("Address outside the valid range");
            int32_t address_b = hex_to_int(tokens[2]);
            if (address_b < 0 || address_b > 4096) throw std::invalid_argument("Address outside the valid range");

            parsed.push_back(Instruction(InstructionByte::AMUL, {address_a, address_b}));
        } else if (tokens[0] == "adiv") {
            int32_t address_a = hex_to_int(tokens[1]);
            if (address_a < 0 || address_a > 4096) throw std::invalid_argument("Address outside the valid range");
            int32_t address_b = hex_to_int(tokens[2]);
            if (address_b < 0 || address_b > 4096) throw std::invalid_argument("Address outside the valid range");

            parsed.push_back(Instruction(InstructionByte::ADIV, {address_a, address_b}));
        } else if (tokens[0] == "aje") {
            int32_t address_a = hex_to_int(tokens[1]);
            if (address_a < 0 || address_a > 4096) throw std::invalid_argument("Address outside the valid range!");
            int32_t address_b = hex_to_int(tokens[2]);
            if (address_b < 0 || address_b > 4096) throw std::invalid_argument("Address outside the valid range!");

            parsed.push_back(Instruction(InstructionByte::AJE, {address_a, address_b}, tokens[3]));
        } else if (tokens[0] == "ajne") {
            int32_t address_a = hex_to_int(tokens[1]);
            if (address_a < 0 || address_a > 4096) throw std::invalid_argument("Address outside the valid range!");
            int32_t address_b = hex_to_int(tokens[2]);
            if (address_b < 0 || address_b > 4096) throw std::invalid_argument("Address outside the valid range!");

            parsed.push_back(Instruction(InstructionByte::AJNE, {address_a, address_b}, tokens[3]));
        } else if (tokens[0] == "inc") {
            int32_t address = hex_to_int(tokens[1]);
            parsed.push_back(Instruction(InstructionByte::INC, {address}));
        } else if (tokens[0] == "amov") {
            int32_t address_a = hex_to_int(tokens[1]);
            if (address_a < 0 || address_a > 4096) throw std::invalid_argument("Address outside the valid range!");
            int32_t address_b = hex_to_int(tokens[2]);
            if (address_b < 0 || address_b > 4096) throw std::invalid_argument("Address outside the valid range!");

            parsed.push_back(Instruction(InstructionByte::AMOV, {address_a, address_b}));
        }
    }
    return {parsed, labels};
}