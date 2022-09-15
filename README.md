# WarpBootstrapCompiler1 🚀 👽

Warp is a branchless functional programming language that is mean to be evaluated at compile time, anything that can/will be executed at run time must explicitly be marked as such

Warp has heavy constraints, the typing scheme is a WIP, it has typed literals but plans are for types to be emergent based on constraints.

This is the second attempt ([re-write](https://github.com/cgbsu/WarpBootstrapCompiler0)) I am making at implementing the compiler, immediate goals are: 

## TODO (Immediate for MVP): 
 - [x] Evaluate literals 🚀
	- [x] Mark bit precision of each variable
	- [x] Make it so fixed-points do not have to specify their base after decimal
	- [x] A few more ways to write fixed-points
 - [x] Evaluate mathematical-expressions (in a type-safe way)
	- [x] Addition/Subtraction
	- [x] Muiltiplication/Division
	- [x] Parenthesis Prioritization
	- [x] Constants (Functions of Zero Parameters)
	- [ ] Function Calls
	- [ ] Bitwise?
 - [x] Evaluate boolean-expressions
	- [ ] Negation
	- [ ] Logical Or
	- [ ] Logical And
	- [ ] Parenthesis Prioritization
 - [ ] Evaluate compile-time functions
 - [ ] Runtime functions & modules

## MVP: 
 - [ ] Parsing
	- [x] Literals
	- [ ] Mathematical Expressions
		- [x] Negation (No Calls/Constants)
		- [x] Negation (Constant Calls)
		- [ ] Negation (Calls)
		- [x] Addition/Subtraction (No Calls/Constants)
		- [x] Addition/Subtraction (Constants Calls)
		- [ ] Addition/Subtraction (Calls)
		- [x] Multiplication/Division (No Calls/Constants)
		- [x] Multiplication/Division (Constant Calls)
		- [ ] Multiplication/Division (Calls)
		- [x] Parenthesis Prioritization
		- [ ] Bitwise?
	- [ ] Boolean expressions
		- [x] Negation (No Calls/Constants)
		- [x] Negation (Constant Calls)
		- [ ] Negation (Calls)
		- [x] Logical Or (No Calls/Constants)
		- [x] Logical Or (Constants Calls)
		- [ ] Logical Or (Calls)
		- [x] Logical And (No Calls/Constants)
		- [x] Logical And (Constant Calls)
		- [ ] Logical And (Calls)
		- [ ] Parenthesis Prioritization
	- [ ] Functions
		- [ ] Function calls
		- [x] Declare constants (functions of zero parameters)
		- [x] Constant calls
 - [ ] Compile Time evaluation
		- [x] Up to current feature set
		- [ ] Of Functions
 - [ ] Compilation
 - [ ] Interopability C/"C ABI"
 - [ ] Interopability C++


## Optimizations

 - [ ] Static caches on `raise`, `raise_constexpr`, `log` and similar math functions
 - [ ] Branching/forking `raise`

## Refactoring

 - [ ] Seperate out everything in NumericLiterals into different classes/files
 - [ ] Find a way to make rules automatically/template rules easily
 - [ ] Find a way to make "optionals"/optional terms in rules 

## Features
- Literals
	- [x] (MVP) Unsigned integers
	- [x] (MVP) Signed integers
	- [x] (MVP) Fixed point values
	- [x] (MVP) Characters
	- [x] (MVP) Bools
	- [x] (MVP) Specify size of data in bits
	- [ ] Specify fixed-point precision
	- [ ] Numerical Deliniator
	- [ ] Specify mixed bases for whole and decimal parts of fixed-points
	- [ ] Complex value type
- [ ] Mathematical Expressions
	- [ ] (MVP) Compile Time
		- [ ] (MVP) Addition/Subtraction
		- [ ] (MVP) Multiplication/Division
		- [ ] (MVP) Parenthesis Prioritization
		- [ ] Raise
		- [ ] Log
		- [ ] (MVP?) Bitshift left/Bitshift right
		- [ ] (MVP?) Bitwise And
		- [ ] (MVP?) Bitwise Or
		- [ ] (MVP?) Bitwise Xor
	- [ ] (MVP) Run Time
		- [ ] (MVP) Addition/Subtraction
		- [ ] (MVP) Multiplication/Division
		- [ ] (MVP) Parenthesis Prioritization
		- [ ] Raise
		- [ ] Log
		- [ ] (MVP?) Bitshift left/Bitshift right
		- [ ] (MVP?) Bitwise And
		- [ ] (MVP?) Bitwise Or
		- [ ] (MVP?) Bitwise Xor

## Longer Term Goals
 - [ ] Numerical Deliniator
 - [ ] Mixed basis for fixed-points/fix the way basis are specified
 - [ ] Specify precision for fixed-points
 - [ ] Floating point types?
 - [ ] Raise/Log
 - [ ] C and C++ interoperability (possibly Rust as well)

# Branchless?

Warp has no if, while, or for loop (or any loops at all!). Instead of using control flow, Warp uses the constraints on function overloads to decide what to do next.

The syntax is likely to change, [but from the original REPL](https://github.com/cgbsu/WarpBootstrapCompiler0), an example: 

```Rust
let factorial( n : n > 0 ) :: n * factorial( n - 1 );
let factorial( n : n = 0 ) @ = 1 :: 1 * 1;
```
`n` is a parameter and what comes after `:` is a constraint, if the constraint is not satisfied for a function the REPL moves on to its next version, if no constriants are satisfied for a function called `factorial` then it results in a compiler error. It is highly deterministic in this way and makes it much easier to have all possible values of the program known at compile time. The `@` represents the return value, so what comes before the `::` are return value constraints (I only put the constarint on the second factorial for demonstration, NOTE: `=` means `equals` in Warp, THERE ARE NO MUTABLE VARIABLES AT COMPILE TIME).

Constraints are just boolean expressions and are therefore composable, with `&&` (logical and) and `||` (logical or), there is even `->` (logical implies)
and `<->` (bi-conditional), and your other familiar comparision operators `<` (less than) `>` (greater than) `<=` (less-than-or-equal-too) `>=` (greater-than-or-equal-to)

Thats pretty much it! The langauge is very simple, that is intentional, here is another example (included in Warp/Math.warp)

```Rust
let raise( base : base > 0, power : power > 0 ) :: base * raise( base, power - 1 );
let raise( base : base > 0, power : power = 0 ) @ = 1 :: 1 * 1;
```

This will raise a number to a certain power.

## Ideas: 
 - [ ] "Solve for" operator (`-->`) can "solve" for parameter or yeild a constant in the function
 - [ ] To resolve overlapping constraints, match too the smallest (satisfied) subset availible.
 - [ ] Make (or find) a "set theory library", have ranges of values and values, when two or more consecutive values are detected add them too a range, when a value is consecutive too a range, check all existing values too see if the range can consume them.
 - [ ] To resolve intersecting sets, take the union of the two sets, then find the smallest satisfied subset.
 - [ ] Module or part of set library that encodes sets in C++ templates.

