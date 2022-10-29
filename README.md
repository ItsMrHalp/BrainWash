# BrainWash
BrainWash - is functional programming language, that is based on brainfuck. It adds only 4 characters, but gives an almost complete set of functional language properties.
BrainWash interpreter is written in C++.

# How to use interpreter

- The first argument should be path to the file with the code. Otherwise you will get an error. 
- The second argument is optional and represents the amount of memory for each function.

# Language basics
1. Any function will have its own memory array.
2. Any function can get 0-255 bytes of memory as input.
3. Any function can return 0-255 bytes of memory as output.
4. The first function to be executed must have the name '+][+'(equivalent to main in c++).

# Language operations
## Comment
'#' - represents a comment and should be posted on both sides
```
   >>+#comment#<>+
```
## Function definition
'c' - makes it possible to create a function.
Any function must have name and code.
- The beginning of the file is considered a global space.
- Everything that fits in the global space up to the first character "c" will be the name of the function (excluding spaces, new lines, tabs and comments).
  The function name can only contain characters from the Brainwash language.
- Everything that fits from the first character "c" to the second character "c" will be the function code.
- Everything that comes after will be considered a global space again.
```
++ c >>++<rc

'++' - name
'>>++<r' - code
```
## Function return
'r' - makes it possible to return from a function.
- Execution of the current function stops.
- The place where the pointer points will be considered the number of bytes to be copied, bytes will be copied starting from the next cell.
- Bytes will be copied in the begining of previous function memory array.
- Pointer will be placed in first cell.

## Function call
'i' - makes it possible to call a function.
- Execution of the current function stops.
- Gets name of the function. The function name should be limited to 'i' on both sides.
- The place where the pointer points will be considered the number of bytes to be copied, bytes will be copied starting from the next cell.
- Bytes will be copied in the begining of new function memory array.
- Pointer will be placed in first cell.
