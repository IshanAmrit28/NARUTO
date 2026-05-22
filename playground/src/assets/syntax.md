# NARUTO Programming Language

> Official Language Documentation

NARUTO is a statically typed, object-oriented, interpreted programming language designed for simplicity, readability, and powerful runtime behavior.

The language combines:
* strict type safety,
* automatic numeric coercion,
* dynamic array behavior,
* and a clean developer-friendly syntax.

NARUTO is built for:
* application development,
* scripting,
* object-oriented systems,
* mathematical computation,
* and educational programming.

---

# Table of Contents

1. Introduction
2. Program Structure
3. Comments
4. Variables
5. Data Types
6. Type System
7. Type Casting
8. Input & Output
9. Operators
10. Conditional Statements
11. Loops
12. Arrays
13. Functions
14. Object-Oriented Programming
15. Structs
16. Memory Model
17. Runtime Behavior
18. Best Practices
19. Full Example Programs

---

# 1. Introduction

## What is NARUTO?

NARUTO is an interpreted programming language that executes code line-by-line through a custom runtime engine.

The language focuses on:
* clean syntax,
* predictable behavior,
* automatic numeric promotion,
* scalable object-oriented architecture,
* and developer readability.

---

## Core Features

### Static Typing
Every variable must declare a type before usage.

### Automatic Numeric Promotion
Numeric expressions automatically widen to preserve precision.

### Object-Oriented Programming
Supports:
* classes,
* constructors,
* inheritance,
* access modifiers,
* method overriding,
* and object instantiation.

### Dynamic Arrays
Arrays automatically resize during assignment.

### Multi-Dimensional Collections
Supports nested arrays of any depth.

### Automatic Input Conversion
The runtime automatically converts input into numeric types when required.

---

# 2. Program Structure

A NARUTO program consists of:
* variable declarations,
* function definitions,
* class definitions,
* expressions,
* and executable statements.

---

## Basic Program

```cpp
print "Welcome to NARUTO";
```

---

## Multi-Line Program

```cpp
int age = 17;
string name = "Naruto";

print name;
print age;
```

---

# 3. Comments

Comments are ignored by the interpreter.

---

## Single Line Comments

```cpp
// This is a comment
print "Hello";
```

---

## Multi-Line Comments

```cpp
/*
    This is a
    multi-line comment
*/
print "Hello";
```

---

# 4. Variables

## What is a Variable?

A variable is a named memory location used to store data during execution.

Each variable contains:
* a type,
* a name,
* and a value.

---

## Variable Declaration Syntax

```cpp
datatype variableName = value;
```

---

## Example

```cpp
int age = 16;
string village = "Leaf";
bool isNinja = true;
```

---

# 5. Data Types

NARUTO supports multiple built-in data types.

---

## Integer Types

### byte

**Description**: An 8-bit signed integer.
**Range**: `-128 to 127`

**Example**:
```cpp
byte level = 10;
print level;
```

---

### short

**Description**: A 16-bit signed integer.

**Example**:
```cpp
short score = 1200;
print score;
```

---

### int

**Description**: A 32-bit signed integer.

**Example**:
```cpp
int chakra = 9000;
print chakra;
```

---

### long

**Description**: A 64-bit signed integer.

**Example**:
```cpp
long worldPopulation = 8000000000;
print worldPopulation;
```

---

## Floating Point Types

### float

**Description**: Stores decimal numbers with standard precision.

**Example**:
```cpp
float speed = 12.5;
print speed;
```

---

### double

**Description**: Stores high precision decimal numbers.

**Example**:
```cpp
double gravity = 9.80665;
print gravity;
```

---

## Text Types

### string

**Description**: Stores sequences of text characters.

**Example**:
```cpp
string name = "Sasuke";
print name;
```

---

### char

**Description**: Stores a single character.

**Example**:
```cpp
char rank = 'A';
print rank;
```

---

## Boolean Type

### bool

**Description**: Stores either true or false.

**Example**:
```cpp
bool alive = true;
print alive;
```

---

# 6. Type System

NARUTO uses static typing. A variable cannot change its structural type after declaration.

---

## Valid Assignment

```cpp
int power = 100;
power = 200;
```

---

## Invalid Assignment

```cpp
int power = 100;
power = "high"; // Error: Cannot assign string to int
```

---

## Numeric Promotion

When numeric types interact, the runtime automatically promotes the result to the most precise type.

**Example**:
```cpp
int a = 10;
float b = 2.5;

// a (int) is implicitly coerced to float
float result = a + b;

print result;
```

---

# 7. Type Casting

Explicit casting converts one type into another type.

---

## Float to Integer

Truncates the decimal value.

```cpp
float damage = 99.9;
int finalDamage = int(damage);

print finalDamage; // Prints 99
```

---

## Number to String

```cpp
int id = 404;
string msg = string(id);

print msg;
```

---

## String to Integer

```cpp
string value = "500";
int power = int(value);

print power;
```

---

# 8. Input & Output

## Output

The `print` keyword outputs expressions to the console.

**Example**:
```cpp
print "Hello";
print 100;
print true;
```

---

## String Concatenation

```cpp
string name = "Naruto";
int age = 17;

print name + " is " + age;
```

---

## Input

The `input()` function reads user input.

**String Input Example**:
```cpp
string name = input("Enter name: ");
print name;
```

---

## Integer Input

Assigning input to an integer automatically converts the user's string text into a number.

```cpp
int age = input("Enter age: ");
print age;
```

---

## Float Input

Assigning input to a float automatically converts the user's string text into a decimal.

```cpp
float weight = input("Enter weight: ");
print weight;
```

---

# 9. Operators

## Arithmetic Operators

```cpp
int a = 10;
int b = 3;

print a + b; // 13
print a - b; // 7
print a * b; // 30
print a / b; // 3
print a % b; // 1
```

---

## Compound Assignment

```cpp
int x = 10;

x += 5; // x is 15
x -= 2; // x is 13
x *= 3; // x is 39
x /= 3; // x is 13
```

---

## Increment & Decrement

```cpp
int value = 5;

value++; // 6
value--; // 5
```

---

## Relational Operators

```cpp
print 10 > 5;   // true
print 10 < 5;   // false
print 10 == 10; // true
print 10 != 5;  // true
```

---

## Logical Operators

```cpp
bool a = true;
bool b = false;

print a && b; // false
print a || b; // true
print !a;     // false
```

---

## Bitwise Operators

```cpp
int flags = 5;

print flags & 1;  // Bitwise AND
print flags | 2;  // Bitwise OR
print flags ^ 3;  // Bitwise XOR
print flags << 1; // Left Shift
print flags >> 1; // Right Shift
```

---

# 10. Conditional Statements

## if Statement

```cpp
int chakra = 50;

if (chakra > 40) {
    print "Ready";
}
```

---

## if else Statement

```cpp
int chakra = 20;

if (chakra > 40) {
    print "Ready";
} else {
    print "Rest";
}
```

---

## else if Chain

```cpp
int level = 3;

if (level == 1) {
    print "Genin";
} else if (level == 2) {
    print "Chunin";
} else {
    print "Jonin";
}
```

---

## Switch Statement

```cpp
int rank = 2;

switch (rank) {
    case 1:
        print "Genin";
    case 2:
        print "Chunin";
    default:
        print "Unknown";
}
```

---

# 11. Loops

## While Loop

```cpp
int i = 0;

while (i < 5) {
    print i;
    i++;
}
```

---

## For Loop

```cpp
for (int i = 0; i < 5; i++) {
    print i;
}
```

---

## Break Statement

Use `break` to exit a loop early.

```cpp
for (int i = 0; i < 10; i++) {
    if (i == 5) {
        break;
    }
    print i;
}
```

---

## Continue Statement

Use `continue` to skip the rest of the current iteration.

```cpp
for (int i = 0; i < 5; i++) {
    if (i == 2) {
        continue;
    }
    print i;
}
```

---

# 12. Arrays

## Array Declaration

Arrays must be declared with a single data type using `[]`.

```cpp
int[] numbers = [1, 2, 3];
```

---

## Accessing Elements

Access elements via their zero-based index.

```cpp
print numbers[0];
```

---

## Updating Elements

```cpp
numbers[1] = 99;
```

---

## Dynamic Expansion

Assigning beyond the array's current size will expand it automatically.

```cpp
numbers[5] = 100;
```

---

## Push Method

Use `.push()` to append a value to the end of an array.

```cpp
numbers.push(500);
```

---

## Array Length

Get the current size of the array.

```cpp
print numbers.length;
```

---

## Multi-Dimensional Arrays

Arrays can be nested to any depth.

```cpp
int[][] matrix = [
    [1, 2],
    [3, 4]
];

print matrix[0][1];
```

---

## Nested Push

You can mutate elements inside nested arrays seamlessly.

```cpp
matrix[1].push(99);
```

---

# 13. Functions

## Function Syntax

```cpp
function returnType functionName(parameters) {

}
```

---

## Simple Function

Use `void` if the function returns nothing.

```cpp
function void greet() {
    print "Hello";
}
```

---

## Function With Parameters

```cpp
function void welcome(string name) {
    print "Welcome " + name;
}
```

---

## Function With Return Value

```cpp
function int add(int a, int b) {
    return a + b;
}
```

---

## Recursive Function

Functions can call themselves.

```cpp
function int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}
```

---

# 14. Object-Oriented Programming

## Class Syntax

```cpp
class Ninja {

}
```

---

## Class Fields

Fields act like variables inside a class.

```cpp
class Ninja {
    string name;
    int chakra;
}
```

---

## Constructor

Constructors run when an object is created and must be named `init()`.

```cpp
class Ninja {
    string name;

    function void init(string n) {
        this.name = n;
    }
}
```

---

## Object Creation

Use the `new` keyword to instantiate a class.

```cpp
Ninja naruto = new Ninja("Naruto");
```

---

## Methods

```cpp
class Ninja {
    string name;

    function void init(string n) {
        this.name = n;
    }

    function void display() {
        print this.name;
    }
}
```

---

## Access Modifiers

Use `private` to hide fields from outside access.

```cpp
class Ninja {
    private int chakra;
}
```

---

## Inheritance

Use `extends` to inherit fields and methods from another class.

```cpp
class Shinobi extends Ninja {

}
```

---

## super Keyword

Use `super` to call a parent class constructor.

```cpp
class Ninja {
    string name;

    function void init(string n) {
        this.name = n;
    }
}

class Shinobi extends Ninja {
    function void init(string n) {
        super(n);
    }
}
```

---

## Method Overriding

Redefining a method in a child class overrides the parent method.

```cpp
class Ninja {
    function void speak() {
        print "Ninja";
    }
}

class Shinobi extends Ninja {
    function void speak() {
        print "Elite Ninja";
    }
}
```

---

# 15. Structs

Structs are lightweight data containers. They cannot have methods or inheritance.

---

## Struct Syntax

```cpp
struct Point {
    int x;
    int y;
}
```

---

## Struct Instantiation

Structs are instantiated without `new`. They use a positional argument syntax.

```cpp
Point p = Point(10, 20);
```

---

## Struct With Arrays

```cpp
struct Team {
    string name;
    int[] scores;
}
```

---

# 16. Memory Model

NARUTO uses:
* stack memory for primitive variables,
* heap memory for objects and arrays.

---

## Stack Variables

```cpp
int age = 10;
```

---

## Heap Objects

```cpp
Ninja n = new Ninja("Naruto");
```

---

# 17. Runtime Behavior

The interpreter executes code:
1. line-by-line,
2. statement-by-statement,
3. scope-by-scope.

---

## Expression Evaluation

Execution order strictly follows mathematical precedence (e.g. multiply before add).

```cpp
// 1. Multiply (5 * 2 = 10)
// 2. Add (10 + 10 = 20)
// 3. Assign
int result = 10 + 5 * 2;
```

---

# 18. Best Practices

* Use meaningful variable names.
* Keep functions focused on one task.
* Prefer smaller classes.
* Avoid deeply nested conditions.
* Use arrays for grouped data.
* Use structs for lightweight storage.

---

# 19. Full Example Programs

## Calculator

```cpp
function int add(int a, int b) {
    return a + b;
}

int result = add(10, 20);
print result;
```

---

## Student System

```cpp
class Student {
    string name;
    int marks;

    function void init(string n, int m) {
        this.name = n;
        this.marks = m;
    }

    function void display() {
        print this.name;
        print this.marks;
    }
}

Student s = new Student("Naruto", 95);
s.display();
```

---

## Multi-Dimensional Data Example

```cpp
int[][] board = [
    [1, 0, 1],
    [0, 1, 0],
    [1, 0, 1]
];

print board[1][1];
```
