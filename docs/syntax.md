# NARUTO Language Syntax Guide

Welcome to the official syntax guide for **NARUTO**, a statically typed, C-inspired programming language. NARUTO brings together the best of both worlds: the strict performance boundaries and object-oriented structure of a language like C++ or Java, combined with the dynamic, developer-friendly implicit type coercion and fluidity of Python.

Whether you are writing simple scripts, complex Object-Oriented systems, or working with dynamic I/O, NARUTO provides a clean, intuitive, and highly powerful syntax.

---

## 1. Introduction to NARUTO

NARUTO executes code line-by-line using a powerful custom-built interpreter. It features:
*   **Static Typing with a Twist:** Variables must have declared types, but mathematical operations will seamlessly auto-promote numbers to prevent data loss.
*   **Object-Oriented Programming:** Full support for Classes, Access Modifiers (`private`), Constructors, and Inheritance (`extends`).
*   **Multi-Dimensional Data:** True support for multi-dimensional nested arrays.
*   **Dynamic I/O:** Built-in `input()` reading that automatically converts strings into numbers.

---

## 2. Variables & Data Types

NARUTO is statically typed, meaning you must explicitly declare the data type of a variable before using it. Once declared, a variable cannot change its structural type (e.g., an `int` cannot become a `string`).

### Supported Data Types & Usage Theory

Choosing the correct data type ensures memory efficiency and prevents unexpected runtime behavior.

#### Integer Types (Whole Numbers)
*   `byte`: An 8-bit integer. **Use case:** Storing very small numbers (e.g., ages, small counters, pixel values).
*   `short`: A 16-bit integer. **Use case:** Storing numbers up to a few thousand (e.g., level progression, days).
*   `int`: A 32-bit integer. **Use case:** The default choice for most whole numbers, counting, and math.
*   `long`: A 64-bit integer. **Use case:** Storing massive numbers like database IDs, global timestamps, or deep mathematical results.

#### Floating Point Types (Decimals)
*   `float`: A standard precision decimal number. **Use case:** Basic physics calculations, percentages, or money.
*   `double`: A high-precision decimal number. **Use case:** High-level scientific calculations or coordinates where precision is paramount.

#### Text & Boolean
*   `string`: A sequence of text characters. **Use case:** Names, text prompts, or data payloads.
*   `char`: A single character. **Use case:** Processing individual letters in algorithms.
*   `bool`: A simple `true` or `false` value. **Use case:** Conditional logic, state tracking, and flags.

### Declaration Examples
```cpp
// Explicit Declarations
int level = 99;
double chakra = 150.55241;
string ninja = "Naruto Uzumaki";
bool isHokage = true;

// Uninitialized Variables (assigned default empty values automatically)
int shadowClones;
string clanName;
```

---

## 3. Type Conversion & Casting

One of NARUTO's most powerful features is its implicit numeric coercion, allowing you to mix and assign numbers without annoying casting boilerplate.

### Implicit Coercion (Widening)
If you perform arithmetic operations on mixed numeric types, NARUTO will automatically "widen" or "promote" the result to the largest, most precise type to prevent data loss.

```cpp
float PI = 3.14;
int radius = 5;

// The integer 'radius' is implicitly coerced to a float.
// The resulting variable 'area' is properly inferred as a float!
float area = PI * radius * radius;
print area;
```

### Explicit Casting
Sometimes you need to forcefully convert a type (e.g., turning a float into an integer by truncating the decimal, or converting a string into a number). You can do this using built-in casting functions.

```cpp
// 1. Casting Float to Int (Truncation)
float rawDamage = 99.9;
int finalDamage = int(rawDamage); 
print finalDamage; // Prints: 99

// 2. Casting Numbers to Strings
int ID = 404;
string errorMsg = "Error " + string(ID) + ": Not Found";

// 3. Casting Strings to Numbers
int power = int("9000");
float exactPower = float("9000.5");
```

---

## 4. Input/Output (I/O)

NARUTO provides extremely powerful built-in functions for handling standard input and output.

### Standard Output (`print`)
The `print` keyword evaluates any expression, outputs it to the console, and automatically appends a newline. NARUTO will automatically coerce numbers to strings if you use the `+` operator!

```cpp
print "Hello World!";
print 123 + 456;    // Prints: 579
print 10 > 5;       // Prints: true

string village = "Hidden Leaf";
int population = 50000;
print "The " + village + " has " + population + " residents."; // Implicitly converts population to string!
```

### Standard Input (`input`)
The `input()` function pauses execution, prints an optional prompt to the console, and captures the user's text. 

**Magic Auto-Conversion:** If you assign the result of `input()` to a numeric variable, NARUTO will internally attempt to parse the string into a number. If the user typed a valid number, it succeeds automatically!

```cpp
// Standard String Input
string user = input("Who are you? ");
print "Welcome, " + user;

// Magic Auto-Conversion to Integer
int age = input("Enter your age: ");
print "Next year you will be: " + (age + 1);

// Magic Auto-Conversion to Float
float weight = input("Enter weight in kg: ");
```

---

## 5. Operators

### Arithmetic
```cpp
int a = 10;
int b = 3;
print a + b; // Addition: 13
print a - b; // Subtraction: 7
print a * b; // Multiplication: 30
print a / b; // Division: 3
print a % b; // Modulo: 1
```

### Compound Assignment & Unary
```cpp
int x = 5;
x += 5; // x is 10
x *= 2; // x is 20
x++;    // x is 21 (Increment)
x--;    // x is 20 (Decrement)
```

### Logical & Relational
Logical operators support **short-circuit evaluation** (e.g., if the left side of `&&` is false, the right side is never executed).

```cpp
bool isStrong = true;
bool isFast = false;

print isStrong && isFast; // false (Logical AND)
print isStrong || isFast; // true  (Logical OR)
print !isStrong;          // false (Logical NOT)

print 10 > 5;  // true
print 5 == 5;  // true
print 5 != 5;  // false
```

### Bitwise
Bitwise operators are specifically restricted to integer types (`byte`, `short`, `int`, `long`).

```cpp
int flags = 5;      // 0101 in binary
print flags & 1;    // 1 (Bitwise AND)
print flags | 2;    // 7 (Bitwise OR)
print flags ^ 15;   // 10 (Bitwise XOR)
print ~flags;       // Bitwise NOT
print flags << 1;   // 10 (Left Shift)
print flags >> 1;   // 2 (Right Shift)
```

---

## 6. Control Flow

### If / Else
```cpp
int chakra = 80;

if (chakra >= 100) {
    print "Ready for Rasenshuriken!";
} else if (chakra >= 50) {
    print "Ready for Rasengan!";
} else {
    print "Need to rest...";
}
```

### Switch Statement
Executes a block of code based on strict equality. Variables declared inside a `case` block are safely block-scoped.

```cpp
int rank = 2;

switch (rank) {
    case 1:
        print "Genin";
    case 2:
        print "Chunin";
    case 3:
        print "Jonin";
    default:
        print "Unknown Rank";
}
```

---

## 7. Loops

### While Loop
```cpp
int clones = 0;
while (clones < 3) {
    print "Shadow Clone Jutsu!";
    clones++;
}
```

### For Loop
Variables declared in the `for` initializer (like `int i`) are isolated to the loop block.

```cpp
for (int i = 0; i < 5; i++) {
    if (i == 2) {
        continue; // Skip the rest of this iteration
    }
    if (i == 4) {
        break; // Exit the loop entirely
    }
    print i;
}
// 'i' no longer exists here!
```

---

## 8. Arrays & Multi-Dimensional Data

Arrays are homogeneous lists (all elements must share the same type). NARUTO supports arrays of any depth (Multi-Dimensional Arrays) and provides built-in methods.

### 1D Arrays
```cpp
// Declaration and Initialization
int[] missions = [10, 20, 30];

// Dynamic resizing via index
missions[5] = 100; // Array automatically expands!

// Built-in methods
string[] names = [];
names.push("Naruto");
names.push("Sasuke");

print names.length; // Prints 2
```

### Multi-Dimensional Arrays
You can nest arrays to create matrices, grids, or complex data structures.

```cpp
int[][] matrix = [[1, 2], [3, 4]];

print matrix[0][0]; // Prints 1

// Pushing to a nested array
matrix[1].push(99); 
print matrix[1][2]; // Prints 99
```

---

## 9. Functions

Functions must explicitly declare their return type and parameter types. Use `void` if the function does not return a value.

```cpp
function int calculateDamage(int base, int multiplier) {
    return base * multiplier;
}

// Recursive function example
function int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

int dmg = calculateDamage(50, 2);
print dmg;
```

---

## 10. Object-Oriented Programming

NARUTO fully supports Classes, Structs, Access Modifiers, and Inheritance.

### Classes and Access Modifiers
Classes encapsulate data and behavior. By default, members are `public`. You can use the `private` keyword to restrict access. The constructor method must be strictly named `init()`.

```cpp
class Ninja {
    string name;
    private int chakra; // Cannot be accessed directly outside the class!

    // The Constructor
    function void init(string n, int c) {
        this.name = n;
        this.chakra = c;
    }

    function void display() {
        print "Ninja: " + this.name + " with Chakra: " + string(this.chakra);
    }
}
```

### Inheritance (`extends` and `super`)
A class can inherit from a parent class. Subclasses must call `super(...)` inside their constructor if the parent requires it.

```cpp
class Shinobi extends Ninja {
    string village;

    function void init(string n, int c, string v) {
        super(n, c); // Call parent 'init' constructor!
        this.village = v;
    }

    function void display() {
        super.display(); // Call parent 'display' method!
        print "Village: " + this.village;
    }
}

Shinobi sasuke = new Shinobi("Sasuke", 90, "Hidden Leaf");
sasuke.display();
```

### Structs
Structs are lightweight data containers meant for grouping simple variables.
- Structs **cannot** have methods or inheritance.
- Structs are instantiated using a Positional Constructor syntax, **NOT** the `new` keyword.

```cpp
struct Location {
    int x;
    int y;
    int[] history; // Structs can hold arrays too!
}

// Instantiate exactly like a function call (positional arguments matching field order)
Location konoha = Location(10, 20, [1, 2, 3]);

print konoha.x;
konoha.history.push(4); // Modifying struct fields works seamlessly
```
