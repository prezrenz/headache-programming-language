# Headache Programming Language

![logo](logo.png)

A hard to use programming language inspired by Scheme.

## Features
- All keywords are made up of special characters. Ex: !! is define.
- Everything is a symbol, and any character can be used as a symbol.
    - This means 1 can be a symbol, that can have any value within.
    - All symbols can have either pairs, numbers, or procedures.
    - The default value of symbols is 0, and defining symbols is the only way to get any numbers to manipulate in the program :)
- No floating points, because I like whole numbers :)
- Negatives exist. For now.

## Syntax
- (\<OPERATOR\> \<OPERANDS\>\*) is the form of most commands. Most commands take many operands. Operands are first evaluated, then the operation is applied.
- (!! \<SYMBOL\>) defines a symbol with the value 0.
- (!! \<SYMBOL\> <INITIAL>) defines a symbol with the value of INITIAL after it has been evaluated.
- (!! (\<PROC NAME\> \<PARAMETERS\>\*) \<BODY\>\*) defines a symbol with a procedure.
- (^^ (\<PARAMETERS\*\>) \<BODY\>\*) returns a procedure but does not define it in the environment.
- (\<+ OR -\>\* \<SYMBOL\>) stacking operators for + and -, which increment and decrement respectively. Stack many of it to increment or decrement a number by that amount. Does not mix with each other.
- (?? \<COND\> \<CONSEQUENT\> \<ALTERNATE\>) checks if cond evaluates to 0, if so then alternate is evaluated and its value returned, otherwise if cond is 1 or greater than 0 then consequent is evaluated and its value returned.
- (\< \>? / \<? / =? \> \<FIRST\> \<SECOND\>) evaluates the expressions FIRST and SECOND then checks if the returned values are greater than, less than, or equal to respectively. Returns 0 for false and 1 for true.
- (! \<EXP\>) evaluates the expression and returns the logical NOT of it.
- ( \<&&/||\> \<EXP\>\*) returns the logical AND or logical OR of the expressions.
- (##) takes an input and returns it as a LIST. The returned list has the ascii values of the inputted characters.
- (@@ \<EXP\>/\<LIST\>) prints the ascii equivalent of the returned number of EXP. Can also print lists, for a string of characters.
- @+, @-, \*\*, //, %% are arithmetic operators that add, subtract, multiply, divide, and modulus respectively its given operands.
- ([] \<LEFT\> \<RIGHT\>) returns a pair with the evaluated value of LEFT and evaluated value of RIGHT. Use with define to store it in the environment.
- (\<]] / [[\> \<PAIR\>) returns the right of a pair and the left of a pair respectively.
- (\<@] / @[\> \<PAIR\> \<VALUE\>) sets the value of the right or left of the pair to the evaluated value of VALUE.
- ([[]] \<EXP\>\*) returns a list out of the evaluated values of EXP.

## Samples
- Defines the symbols 1 and 2 and increment their values from 0 to 1 and 2 respectively
```
(!! 1)
(+ 1)
(!! 2)
(++ 2)
(+ 1)
(@@ (@+ 1 2))
```

## References
- [Scheme from Scratch by Peter Michaux](http://peter.michaux.ca/articles/scheme-from-scratch-introduction)
- [Structure and Interpretation of Computer Programs](https://www.amazon.com/Structure-Interpretation-Computer-Programs-Engineering/dp/0262510871)
- Logo is from [game-icons.net](game-icons.net)
