# 🍥 Naruto Language

**Naruto** is a custom, statically typed, interpreted programming language built entirely in **C++**.

This project was born out of a desire to move beyond using C++ solely for Data Structures & Algorithms (DSA) and to explore the engineering behind compiler design and language architecture.

## ✨ Features

- **Statically Typed:** Catches type errors before runtime.
- **Rich Type System:** Inspired by **Java**, supporting `byte`, `short`, `int`, `long`, `float`, `double`, `bool`, `char`, and `string`.
- **Python-Style Input:** Smart `input()` function that automatically detects and converts user input to the appropriate type (Int, Float, or String).
- **Control Flow:** Full support for:
  - `if` / `else`
  - `while` loops
  - `for` loops (C-style)
  - `switch` / `case` (with fall-through support)
  - `break` and `continue`
- **Recursion:** Capable of handling recursive function calls (e.g., Fibonacci, Factorial).
- **Arrays:** Fixed-size, homogeneous arrays.
- **Bitwise Operations:** Low-level manipulation (`&`, `|`, `^`, `<<`, `>>`).

## 🛠️ Architecture

The language is built using a classic interpreter pipeline:

1.  **Lexer:** Tokenizes the source code.
2.  **Parser:** Generates an Abstract Syntax Tree (AST).
3.  **Type Checker:** Validates semantic correctness and type safety.
4.  **Interpreter:** Traverses the AST and executes the logic.

## 🚀 Getting Started

### Prerequisites

- A C++ Compiler (GCC/G++ recommended)

### Build

Clone the repository and compile the source code:

```bash
g++ src/main.cpp -o naruto
```
