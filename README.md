# WarpBootstrapCompiler1 🚀 👽

Warp is a branchless functional programming language that is mean to be evaluated at compile time, anything that can/will be executed at run time must explicitly be marked as such

Warp has heavy constraints, the typing scheme is a WIP, it has typed literals but plans are for types to be emergent based on constraints.

This is the second attempt ([re-write](https://github.com/cgbsu/WarpBootstrapCompiler0)) I am making at implementing the compiler, immediate goals are: 

- [] Evaluate literals
- [] Evaluate mathematical-expressions (in a type-safe way)
- [] Evaluate boolean-expressions
- [] Evaluate compile-time functions
- [] Runtime functions & modules


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

