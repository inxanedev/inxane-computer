#pragma once

#include <vector>
#include <string>


enum class InstructionByte {
    EXIT       = 0x00,
    MOV        = 0x01,
    ADD        = 0x02,
    SUB        = 0x03,
    MUL        = 0x04,
    DIV        = 0x05,
    JE         = 0x06,
    JNE        = 0x07,
    JMP        = 0x08,
    AOUT       = 0x09,
    ASCII_AOUT = 0x0A,
    OUT        = 0x0B,
    ASCII_OUT  = 0x0C
};

class Instruction {
    public:
        Instruction(InstructionByte instruction, std::vector<int32_t>&& arguments)
            : instruction(instruction), arguments(arguments) {}
        
        Instruction(InstructionByte instruction) : instruction(instruction), arguments({}) {}

        Instruction(InstructionByte instruction, std::vector<int32_t>&& arguments, const std::string& o_label_argument)
            : instruction(instruction), arguments(arguments), o_label_argument(o_label_argument) {}

        InstructionByte instruction;
        std::vector<int32_t> arguments;
        std::string o_label_argument;
};

using parsed_t = std::pair<std::vector<Instruction>, std::vector<std::pair<std::string, uint32_t>>>;

parsed_t parse_source(const std::vector<std::string>& source);