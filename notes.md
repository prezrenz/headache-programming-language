# Project Headache

A purely symbolic, no keywords, lisp like language.

All commands start and end in parens. It follows the pattern (operator (operand)+). Operator is a statement, operand is an expression.

(!! Symbol *(command)+) Defines a symbol and inits it with value 0. If there's a lambda, this defines a function that calls the command. Also assigns value.

(^^ ((arg)*) ((command)*)) Lambda, returns a function object.

(+ Symbol) Binary increments the value in the symbol. If it's a function, throws an error. Stacks endlessly to increment a lot in just 1 command.

(- Symbol) Binary decrements. Decrementing from 0 will loop back to highest possible number. Stacks same as above.

(Symbol symbol) Assigns the value of symbol to symbol, or if it's a function, will assign the lambda of the function. If there's no return value, it throws error.

(>? Symbol symbol2 (command)+) Will compare symbol with symbol2, if symbol is greater than symbol2.

(<? Symbol symbol2 (command)+) Same as above but for less than.

(=? Symbol symbol2 (command)+) Same as above but will check if equals.

(// Symbol symbol2) Divides symbol by symbol2 and returns the value. Throws error if one is function.

(** Symbol symbol2) Same as above but for multiplication.

(@+ Symbol symbol2) Adds both and returns the value.

(@- Symbol symbol2) Same as above but subtract.

(<< *(Symbol)) Returns a function. Using this outside a function will exit the program prematurely. Returns an optional symbol.

(%% Symbol) Asks for a single character input and stores it as a hex value inside symbol.

(@@ Symbol) Writes the binary value of Symbol into output as an ascii character.

## OPTIONAL

(!! Symbol []) Defines a symbol that contains a dynamic array. Symbol will return the value of the first value in array, which is 0 upon definition.

(+ Array) Moves the array pointer to the right by one. Stacks. Arrays have a max value of 255.

(- Array) Same as above but moves left. Loops back to pointing to 255 if going below 0.

(Array Symbol) Stores the value inside symbol into Array. If function, will store the return value.

## Data Types
  - uint8
  - function
  - uint8 array[256]

### Symbol Struct
  - Union uint8, uint8 array[256], array of pointers

### Op Struct
  - String Op
  - int OpNum
  - array of Op Struct

## Expressions
  - Symbol - evaluates to the value inside, if it's a function will return return value if it's array will return the value at head
  - Function -

## Code Structure
  - Scanner: make tokens, like with Statement Lang

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
- Everything is a symbol.
- Symbols contain different datas and are declared differently each.
    - ([] SYMBOL) declares a symbol with the type array. The whole array has an initial value of 0.
    - (^^ SYMBOL (<params>) <body>) declares a symbol with the type function, with a list of params followed by a list of commands.
    - (!! SYMBOL) declares a symbol with the type number. Its initial value is 0.
    - Evaluating these commands will return a number at the end, either 0 on success or 1 on fail.
- Commands are represented as lists. Lists are made as pairs with left and right, with left containing the data and right pointing to the next item on the list.
- Entering a symbol will look for that symbol and print its value. If its an array, the whole array is printed with its values and index and current index. If it's a number, it will print the number. If it's a procedure, it will print either <primitive proc> or <compound proc>
- You can use your entire keyboard to name symbols.

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
- [ ] Implement environment
    - [x] Read Section 3.2 of SICP
    - [x] Read Section 4.1.3 of SICP
    - [x] Write more tasks, outline the algorithm for environment implementation
    - [x] Write environment as a pair, with left pointing to a frame and right pointing to the enclosing environment. If the enclosing environment is the empty list then current environment is the global environment. A frame is a list of vars and corresponding vals, with the left having the vars and the right being a list of values.
    - [x] Implement making a frame, adding variables, extending environment, and looking up values
    - [ ] Implement defining symbols, with initial value of 0.
        - [ ] Implement symbol table and add !! to it.
        - [ ] Implement evaluating of symbols and !! define
    - [ ] Implement printing the value of the defined symbol
