#pragma once

#include <vector>
#include <string>


enum class InstructionByte {
    EXIT,
    MOV,
    ADD,
    SUB,
    MUL,
    DIV,
    JE,
    JNE,
    JMP,
    AOUT,
    ASCII_AOUT,
    OUT,
    ASCII_OUT,
    AADD,
    ASUB,
    AMUL,
    ADIV,
    AJE,
    AJNE,
    INC,
    AMOV,
    MOD,
    AMOD
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