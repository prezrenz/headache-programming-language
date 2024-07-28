# Project Headache

A purely symbolic, no keywords, lisp like language.

All commands start and end in parens. It follows the pattern (operator (operand)+). Operator is a statement, operand is an expression.

## Grammar

operation = "(" operator (operand)*)
operator = "[[" / "]]" / "[]" / "!!" / "@@" / ("+")* / ("-")* / ">?" / "<?" / "=?" / "@-" / "@+" / "//" / "**" / "%%" / "<<" / "^^" / symbol
operand = symbol / operation
symbol = NUMBER / FUNCTION / PAIR

## Grammar

expression = SYMBOL / list
list "(" (command)* ")"
command = procedure (argument)+
procedure = SYMBOL
argument = SYMBOL / command

## Features
- Everything is a symbol. Nearly everything has a return value.
- Symbols contain different datas and are declared differently each.
    - ([] <SYMBOL NAME> <SYMBOL LEFT> <SYMBOL RIGHT>) declares a PAIR with the name SYMBOL NAME and points to a declared symbol in the environment with name SYMBOL LEFT and name SYMBOL RIGHT.
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
- The (]] <PAIR SYMBOL>) return the left symbol pointed by left of pair.
- The ([[ <PAIR SYMBOL>) return the right symbol pointed by right of pair.
- ([[]] <SYMBOL>*) takes a variable amount of SYMBOLs and returns a list out of it.
- (&& <SYMBOL> <SYMBOL>) and (|| <SYMBOL> <SYMBOL>) logically ANDs or ORs the two symbols respectively, they are primitive functions.
- @+, @-, //, and ** all follow a pattern (OP <SYMBOL>*) and are primitive functions that take a variable amount of symbols and adds, subtracts, divides, and multiplies their values respectively. If the values returned by these symbols are not numbers, then an error is thrown. %% returns the remainder. Division by default returns the quotient in whole numbers.
- (~~ <SYMBOL>) logically NOTs the value of the symbol.
- (@] <SYMBOL> <SYMBOL VALUE>) and (@[ <SYMBOL> <SYMBOL VALUE>) assigns the value of VALUE SYMBOL to SYMBOL, with @] assigning to right of pair and @[ assigning to right.
- (!! <SYMBOL> (VALUE)) defines a new symbol, with an optional VALUE. VALUE is evaluated and assigned to the new symbol. If VALUE is ommited, the SYMBOL is assigned the number value 0. Calling !! on an existing SYMBOL overwrites its current value to either 0 or VALUE.
- (@@ <SYMBOL>) writes the symbol evaluated as an ascii character. Takes lists to write strings.
- (## <SYMBOL>) asks user for input and returns it as a list.
- (^^ (<SYMBOL>*) <COMMANDS>*) takes a list of SYMBOLS defined as arguments in a new environment when called, with the called parameters as values, and a list of COMMANDS that is evaluated one after another when called.

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
- [x] Implement ?? if command
    - [x] Implement checking if symbol is 0 or 1
- [x] Implement conditionals
    - [x] Implement >?
    - [x] Implement <?
    - [x] Implement =?
- [-] Implement array pointer movement left and right
    - [x] Remove arrays
- [x] Refactor and split out all code
- [x] Refactor eval function
- [x] Convert !! define to accomodate any data type, if 3rd arg is empty defaults to num
- [x] Implement primitive functions
    - [x] Implement @+ addition
    - [x] Implement @- subtraction
    - [x] Implement // division
    - [x] Implement ** multiplication
    - [x] Implement %% modulo
- [x] Implement pairs
    - [x] Implement [] making pairs
    - [x] Implement ]] getting pair right
    - [x] Implement [[ getting pair left
    - [x] Implement @[ set pair left
    - [x] Implement @] set pair right
    - [x] Implement [[]] list
- [x] Implement I/O
    - [x] Implement @@ printing
        - [x] Implement printing lists
    - [x] Implement ## taking line input and returning the input as list
- [x] Don't print after eval when running a file.
- [x] Implement ^^ compound functions
- [x] Implement logical operators
    - [x] Implement && logical AND
    - [x] Implement || logical OR
    - [x] Implement ~~ logical NOT (primitive proc)
- [x] Make headache logo (head game-icon with red)
- [x] Make README Markdown
    - [x] Write Features
    - [x] Write samples and outputs
    - [x] Write References
- [ ] Fix bugs
    - [ ] reading: entering ) enters an endless loop
    - [ ] reading: entering ( followed by anything else constantly asks for input
    - [ ] reading: entering (sym)bol will leave bol in buffer (optional)
    - [ ] reading: exiting with <C-c> gives error variable undefined error
    - [ ] input/output proc: print is somehow reading an empty list returned from input
    - [ ] stacking: Validate if objects operated on are numbers
    - [ ] stacking: What happens if you define + or - as symbols?
    - [ ] input/output proc: its printing the empty list, should not
    - [ ] test and/or/not
#### Optional Tasks
- [ ] Implement Program Loading
- [ ] Implement $$ make string as list special form
- [ ] Refactor and clean
    - [ ] Give names to pair commands e.g. proc = eval(gpl(obj)) what does gpl mean in this context?
    - [ ] make type checks for each data type and use that everywhere
    - [ ] reorder everything according to their use
    - [ ] Replace all symbol checking with is_tagged_list
- [ ] Reimplement conditionals as primitives
    - [ ] Implement >?
    - [ ] Implement <?
    - [ ] Implement =?
