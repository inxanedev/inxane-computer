
# inxane-computer
This is the github repository of my project, "inxane-computer".
It consists of two C++ projects - the computer simulator, and the compiler for that computer.
## The simulator
The simulator can execute the programs compiled using the... well, compiler.
The compiler outputs a binary, and the simulator can execute the instructions within it.
It's not complicated, all it does is allocates some memory for the execution of the program, and reads it byte by byte. Then, it checks what instruction it is, gets its arguments, and executes it.

The simulator has 16384 bytes of memory. Each memory address corresponds to a 32-bit integer, and because a 32-bit integer is 4 bytes in size, the computer has exactly 4096 available addresses to store memory. The addresses therefore range from 0x0000 to 0x1000.
## The language
The language that the compiler understands is very simple.
Each instruction is on its own line, and the list of instructions is as follows:
*by the way, ADDRESSes are written in hex, for example 0x0001, two bytes.*
- `exit` - terminates the execution
- `mov ADDRESS VALUE` - sets the value at the ADDRESS to the specified value
- `add ADDRESS VALUE` - adds the VALUE to the value at the ADDRESS
- `sub ADDRESS VALUE` - subtracts the value at the ADDRESS from the VALUE
- `mul ADDRESS VALUE` - multiplies the value at the ADDRESS by the VALUE
- `div ADDRESS VALUE` - divides the value at the ADDRESS by the VALUE
- `jmp LABEL` - jumps to the label unconditionally (we'll talk about labels soon)
- `je ADDRESS VALUE LABEL` - jumps to the LABEL if the value at the ADDRESS is equal to VALUE
- `jne ADDRESS VALUE LABEL` - jumps to the LABEL if the value at the ADDRESS is not equal to VALUE
- `aout ADDRESS` - outputs the value at the ADDRESS as a number
- `ascii-aout ADDRESS` - outputs the value at the ADDRESS as an ASCII character
- `out VALUE` - outputs the VALUE as a number
- `ascii-out VALUE` - outputs the VALUE as an ASCII character
- `aadd ADDRESS ADDRESS` - adds the value at the second address to the value at the first address
- `asub ADDRESS ADDRESS` - same as `aadd` but subtraction
- `amul ADDRESS ADDRESS` - same as `aadd` but multiplication
- `adiv ADDRESS ADDRESS` - same as `aadd` but division
- `aje ADDRESS ADDRESS LABEL` - if values of the two ADDRESSes are the same, jump to the LABEL
- `ajne ADDRESS ADDRESS LABEL` - if values of the two ADDRESSes are not the same, jump to the LABEL

There are other lexical constructs in the language, such as labels.
Labels are created by the 'instruction' (it's in quotes because it's not an actual instruction that is inside the binary, it's only for the compiler) with the obvious name of `label`, followed by the name of that label.
After creating a label we can use it to jump to that label, for example with `jmp loop`, if we've defined a label called loop. This, along with the `jmp` family of instructions, makes simple control flow possible.

There's another thing which is the `print <text>` instruction, which the compilers transforms into a bunch of `ascii-out` instructions.

There is a big issue with my language's design, and that is that labels CANNOT be used before they're declared. You cannot jump to a label that is declared later in the code. This is unfortunately a limitation, but it doesn't make the language impossible to use.
# Simulator details
All the memory available to the program gets set to `0` using `memset`, so you can safely assume that all valid addresses in your program, from `0x0000` to `0x1000` will all have the value `0`. Therefore, you don't have to write a `mov` instruction to set anything to 0 at the beginning of the program.

The instructions `aout, ascii-aout, out`, and `ascii-out` are non-standard, meaning that they don't have to be implemented by other simulators, they're only guaranteed to be implemented in the simulator in this repository.

## Usage
### Compiler
`./compiler-binary "program.ipc" "output.bin"`
### Simulator
`./simulator-binary "output.bin"`

# Example programs
### Print numbers from 1 to 10
    label loop
    add 0x0000 1
    aout 0x0000
    ascii-out 10
    jne 0x0000 10 loop
    exit
  We're gonna be looping, so we need a loop. We can do that via a label, so we create a label at the beginning, so we can jump to it later. Then, we add 1 to the memory location `0x0000`. Then, we output that value. In order to display each number on a new line, there's an instruction `ascii-out 10`, which prints a newline (newline is 10 in the ASCII table). Then, we check if we've reached 10 already, if we did we simply skip the instruction as if nothing happened, and exit the program. But if we didn't reach it yet, we jump back to the loop and run the program again, but this time, `0x0000` is gonna be of value `1`. And so on.
