
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

There are other lexical constructs in the language, such as labels.
Labels are created by the 'instruction' (it's in quotes because it's not an actual instruction that is inside the binary, it's only for the compiler) with the obvious name of `label`, followed by the name of that label.
After creating a label we can use it to jump to that label, for example with `jmp loop`, if we've defined a label called loop. This, along with the `jmp` family of instructions, makes simple control flow possible.

*TO BE CONTINUED, I GOTTA GO SOMEWHERE*
