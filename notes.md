# Project Headache

A purely symbolic, no keywords, lisp like language.

All commands start and end in parens. It follows the pattern (operator (operand)+). Operator is a statement, operand is an expression.

(Symbol symbol) Assigns the value of symbol to symbol, or if it's a function, will assign the lambda of the function. If there's no return value, it throws error.

(// Symbol symbol2) Divides symbol by symbol2 and returns the value. Throws error if one is function.

(** Symbol symbol2) Same as above but for multiplication.

(@+ Symbol symbol2) Adds both and returns the value.

(@- Symbol symbol2) Same as above but subtract.

(<< *(Symbol)) Returns a function. Using this outside a function will exit the program prematurely. Returns an optional symbol.

(%% Symbol) Asks for a single character input and stores it as a hex value inside symbol.

(@@ Symbol) Writes the binary value of Symbol into output as an ascii character.

## Data Types
  - int
  - function
  - int array[256]

## Expressions
  - Symbol - evaluates to the value inside, if it's a function will return return value if it's array will return the value at head
  - Function -

## Grammar

operation = "(" operator (operand)*)
operator = "!!" / "@@" / ("+")* / ("-")* / ">?" / "<?" / "=?" / "@-" / "@+" / "//" / "**" / "%%" / "<<" / "^^" / symbol
operand = symbol / operation
symbol = uint8 / function / uint8 array[256]

## Grammar

expression = SYMBOL / list
list "(" (command)* ")"
command = procedure (argument)+
procedure = SYMBOL
argument = SYMBOL / command

## Features
- Everything is a symbol. Nearly everything has a return value.
- Symbols contain different datas and are declared differently each.
    - ([] SYMBOL) declares a symbol with the type array. The whole array has an initial value of 0.
    - (^^ SYMBOL (<params>) <body>) declares a symbol with the type function, with a list of params followed by a list of commands.
    - (!! SYMBOL) declares a symbol with the type number. Its initial value is 0.
    - Evaluating these commands will return a number at the end, either 0 on success or 1 on fail.
- Commands are represented as lists. Lists are made as pairs with left and right, with left containing the data and right pointing to the next item on the list.
- Entering a symbol will look for that symbol and print its value. If its an array, the whole array is printed with its values and index and current index. If it's a number, it will print the number. If it's a procedure, it will print either <primitive proc> or <compound proc>
- You can use almost any character for symbols.
- Only 0 is false, any none 0 value is true.
- The 4 checks return 0 or 1 for true or false.
- The (?? <cond> <if> <else>) will evaluate if cond is 0 or 1 and execute <if> if 1 and <else> if 0.
- The (>? <SYMBOL> <SYMBOL>), (>? <SYMBOL> <SYMBOL>), and (=? <SYMBOL> <SYMBOL>) compares if the two symbols are greater than, less than, or equal respectively and return 1 or 0 depending on the result.
- The (%% <SYMBOL>) will take a single character input and store its value inside the symbol as its ascii representation. If it's an array, it is stored in the current pointed index.
- (] <SYMBOL>) and ([ <SYMBOL>) moves the pointer in an array that SYMBOL refers to right and left respectively, and also stacks. If the index turns to less than 0, it goes to 0, if it goes to more than MAX_ARRAY, it goes back to MAX_ARRAY.

### Tasks
- [x] Read input as list.
    - Read function: Eat ( or read symbol, go to Read pair if found
    - Read pair: check if ), return empty list if found
    - Read pair: otherwise, call Read function
    - Read function: if while current character is not whitespace/null terminator/ ) then add character to buffer
    - Read function: on encounter whitespace, stop and return as symbol the buffer, if null or ) then error
    - Read pair: on recieving left, go to right and read pair again
- [x] Print the returned list as a continuous list.
    - Print groupings properly
- [ ] Fix reading bugs
    - [ ] entering ) enters an endless loop
    - [ ] entering ( followed by anything else constantly asks for input
    - [ ] entering (sym)bol will leave bol in buffer (optional)
    - [ ] exiting with <C-c> gives error variable undefined error
- [x] Implement environment
    - [x] Read Section 3.2 of SICP
    - [x] Read Section 4.1.3 of SICP
    - [x] Write more tasks, outline the algorithm for environment implementation
    - [x] Write environment as a pair, with left pointing to a frame and right pointing to the enclosing environment. If the enclosing environment is the empty list then current environment is the global environment. A frame is a list of vars and corresponding vals, with the left having the vars and the right being a list of values.
    - [x] Implement making a frame, adding variables, extending environment, and looking up values
    - [x] Implement defining symbols, with initial value of 0.
        - [x] Implement symbol table and add !! to it.
        - [x] Implement evaluating of symbols and !! define
    - [x] Implement printing the value of the defined symbol
- [x] Implement array definition
- [ ] Implement stacking + and -
    - [x] Add stacking to struct and enum, with len
    - [x] Read + or - and error if any other symbol in string, stop on delimiter and store len
    - [x] In eval pair, check if stacking + or - enum type, if so call set var and add len to var
    - [x] Return lookup var val
    - [ ] Validate if objects operated on are numbers
    - What happens if you define + or - as symbols?
- [x] Implement ?? if command
    - [x] Implement checking if symbol is 0 or 1
- [ ] Implement conditionals
    - [ ] Implement >?
    - [ ] Implement <?
    - [ ] Implement =?
- [ ] Implement array pointer movement left and right
