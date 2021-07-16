#include "compiler.hh"

#include <ios>
#include <stdexcept>
#include <unordered_map>
#include <fstream>

#define push_instruction() output_bytes.push_back(static_cast<uint8_t>(ins.instruction))
#define push_address() auto address_bytes = split_address(ins.arguments[0]); \
            output_bytes.push_back(address_bytes.first); \
            output_bytes.push_back(address_bytes.second)
#define push_arithmetic() auto arg_bytes = split_int32(ins.arguments[1]); \
            for (uint8_t byte : arg_bytes) { \
                output_bytes.push_back(byte); \
            }

std::pair<uint8_t, uint8_t> split_address(int32_t address) {
    uint8_t* bytes = reinterpret_cast<uint8_t*>(&address);
    /*
        0xAABB -> 0x0000AABB
        as array: [BB, AA, 00, 00]
        
        return [1], [0]
    */
    return {bytes[1], bytes[0]};
}

std::vector<uint8_t> split_int32(int32_t value) {
    uint8_t* bytes = reinterpret_cast<uint8_t*>(&value);
    return {bytes[3], bytes[2], bytes[1], bytes[0]};
}

std::pair<std::string, uint32_t> search_label(const std::string& label, const std::vector<std::pair<std::string, uint32_t>>& labels) {
    for (int i = 0; i < labels.size(); i++) {
        if (labels[i].first == label) {
            return labels[i];
        }
    }
    throw std::out_of_range("Label not found!");
}

void compile(const parsed_t& parsed, const std::string& output_filename) {
    std::vector<Instruction> instructions = std::move(parsed.first);
    std::vector<std::pair<std::string, uint32_t>> labels = std::move(parsed.second);

    std::vector<uint8_t> output_bytes;

    std::unordered_map<std::string, unsigned int> label_offsets;

    unsigned int byte_counter = 0;

    for (int i = 0; i < instructions.size(); i++) {
        Instruction ins = std::move(instructions[i]);

        for (int j = 0; j < labels.size(); j++) {
            if (labels[j].second == i) {
                label_offsets.insert({labels[j].first, byte_counter});
            }
        }

        if (ins.instruction == InstructionByte::EXIT) {
            push_instruction();
            byte_counter += 1;
        } else if (ins.instruction == InstructionByte::MOV) {
            // mov 0x0000 1234
            // 0x01 0x00 0x00 0x00 00 04 D2
            push_instruction();
            push_address();
            push_arithmetic();

            byte_counter += 1 + 2 + 4;
        } else if (ins.instruction == InstructionByte::ADD) {
            push_instruction();
            push_address();
            push_arithmetic();

            byte_counter += 1 + 2 + 4;
        } else if (ins.instruction == InstructionByte::SUB) {
            push_instruction();
            push_address();
            push_arithmetic();

            byte_counter += 1 + 2 + 4;
        } else if (ins.instruction == InstructionByte::MUL) {
            push_instruction();
            push_address();
            push_arithmetic();

            byte_counter += 1 + 2 + 4;
        } else if (ins.instruction == InstructionByte::DIV) {
            push_instruction();
            push_address();
            push_arithmetic();

            byte_counter += 1 + 2 + 4;
        } else if (ins.instruction == InstructionByte::JMP) {
            push_instruction();

            std::pair<std::string, uint32_t> label = search_label(ins.o_label_argument, labels);
            auto offset_bytes = split_int32(label_offsets.at(label.first));

            for (uint8_t byte : offset_bytes) {
                output_bytes.push_back(byte);
            }

            byte_counter += 1 + 4;
        } else if (ins.instruction == InstructionByte::JE || ins.instruction == InstructionByte::JNE) {
            push_instruction();
            push_address();

            auto value_bytes = split_int32(ins.arguments[1]);

            for (uint8_t byte : value_bytes) {
                output_bytes.push_back(byte);
            }

            std::pair<std::string, uint32_t> label = search_label(ins.o_label_argument, labels);
            auto offset_bytes = split_int32(label_offsets.at(label.first));

            for (uint8_t byte : offset_bytes) {
                output_bytes.push_back(byte);
            }

            byte_counter += 1 + 2 + 4 + 4;
        } else if (ins.instruction == InstructionByte::AOUT || ins.instruction == InstructionByte::ASCII_AOUT) {
            push_instruction();
            push_address();

            byte_counter += 1 + 2;
        } else if (ins.instruction == InstructionByte::OUT) {
            push_instruction();

            auto value_bytes = split_int32(ins.arguments[0]);
            for (uint8_t byte : value_bytes) {
                output_bytes.push_back(byte);
            }

            byte_counter += 1 + 4;
        } else if (ins.instruction == InstructionByte::ASCII_OUT) {
            push_instruction();

            auto value_bytes = split_int32(ins.arguments[0]);
            output_bytes.push_back(value_bytes[3]);

            byte_counter += 1 + 1;
        } else if (ins.instruction == InstructionByte::AADD || ins.instruction == InstructionByte::ASUB || ins.instruction == InstructionByte::AMUL || ins.instruction == InstructionByte::ADIV) {
            push_instruction();

            auto address_a = split_address(ins.arguments[0]);
            auto address_b = split_address(ins.arguments[1]);

            output_bytes.push_back(address_a.first);
            output_bytes.push_back(address_a.second);
            output_bytes.push_back(address_b.first);
            output_bytes.push_back(address_b.second);

            byte_counter += 1 + 2 + 2;
        } else if (ins.instruction == InstructionByte::AJE || ins.instruction == InstructionByte::AJNE) {
            push_instruction();
            auto address_a = split_address(ins.arguments[0]);
            auto address_b = split_address(ins.arguments[1]);
            output_bytes.push_back(address_a.first);
            output_bytes.push_back(address_a.second);
            output_bytes.push_back(address_b.first);
            output_bytes.push_back(address_b.second);

            std::pair<std::string, uint32_t> label = search_label(ins.o_label_argument, labels);
            auto offset_bytes = split_int32(label_offsets.at(label.first));

            for (uint8_t byte : offset_bytes) {
                output_bytes.push_back(byte);
            }

            byte_counter += 1 + 2 + 2 + 4;
        }
    }

    std::ofstream output_file(output_filename, std::ios::binary | std::ios::out);
    output_file.write(reinterpret_cast<const char*>(output_bytes.data()), output_bytes.size());
}