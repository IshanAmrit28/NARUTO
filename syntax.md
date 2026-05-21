# NARUTO Language Syntax Guide

Welcome to the official syntax guide for **NARUTO**, a statically typed, C-inspired programming language featuring Object-Oriented capabilities, dynamic Python-like type coercion, and robust control flows. 

NARUTO was designed to provide the performance and structure of a statically typed language (like C++ or Java) while maintaining the developer-friendly flexibility of dynamically typed languages (like Python). Whether you are writing simple scripts, complex Object-Oriented systems, or working with dynamic I/O, NARUTO provides a clean, intuitive syntax.

---

## 1. Variables & Data Types

NARUTO is statically typed, meaning variables must be declared with a specific type. However, it also features implicit numeric coercion (widening), meaning you can seamlessly mix and assign numeric types similar to dynamically typed languages.

### Supported Types
- **Integers**: `int`, `byte`, `short`, `long`
- **Floating Point**: `float`, `double`
- **Text**: `string`, `char`
- **Boolean**: `bool` (`true` or `false`)

### Declaration
```cpp
// Explicit Declarations
int level = 99;
float chakra = 150.5;
string ninja = "Naruto";
bool isHokage = true;

// Uninitialized Variables (assigned default values)
int shadowClones;
```

---

## 2. Type Conversion & Casting

NARUTO handles numeric types dynamically behind the scenes, offering both implicit coercion and explicit casting functions.

### Implicit Coercion (Python-like)
NARUTO supports implicit coercion across all numeric types. For example, an `int` and a `float` can be safely added together, and the result will seamlessly promote to a `float`.

```cpp
float PI = 3.14;
int radius = 5;

// radius (int) is implicitly coerced to float for the calculation!
float area = PI * radius * radius;
print area;
```

### Explicit Casting
You can explicitly cast values using built-in type conversion functions. These act exactly like function calls.

```cpp
// Cast float to int (truncates the decimal)
int floored = int(3.99); // becomes 3

// Cast int to string
string name = string(101); // becomes "101"

// Cast string to int or float
int power = int("9000");
float exactPower = float("9000.5");
```

---

## 3. Input/Output (I/O)

NARUTO provides simple built-in keywords and functions for handling standard input and output.

### Standard Output (`print`)
The `print` keyword outputs data to the console, followed automatically by a newline. It can print any data type.

```cpp
print "Hello World!";
print 123;
print true;

string village = "Hidden Leaf";
print "Welcome to the " + village;
```

### Standard Input (`input`)
The `input()` function pauses execution, prints an optional prompt to the console, and captures user text. 
Because of NARUTO's implicit coercion, the returned string is seamlessly cast to numbers if assigned directly to numeric variables!

```cpp
string user = input("Who are you? ");
print "Welcome, " + user;

// Seamless type coercion at work!
int age = input("Enter your age: ");
print age + 10;
```

---

## 4. Operators

NARUTO supports a full suite of standard operators.

### Arithmetic
```cpp
int a = 10;
int b = 3;
print a + b; // 13
print a - b; // 7
print a * b; // 30
print a / b; // 3
print a % b; // 1
```

### Compound Assignment & Unary
```cpp
int x = 5;
x += 5; // x is 10
x *= 2; // x is 20
x++;    // x is 21
x--;    // x is 20
```

### Logical & Relational
Logical operators support **short-circuit evaluation** (e.g., if the left side of `&&` is false, the right side is not evaluated).
```cpp
bool isStrong = true;
bool isFast = false;

print isStrong && isFast; // false
print isStrong || isFast; // true
print !isStrong;          // false (Logical NOT)

print 10 > 5;  // true
print 10 <= 5; // false
print 5 == 5;  // true
print 5 != 5;  // false
```

### Bitwise
```cpp
int flags = 5;      // 0101
print flags & 1;    // 1 (0001)
print flags | 2;    // 7 (0111)
print flags ^ 15;   // 10
print ~flags;       // Bitwise NOT (Two's Complement)
print flags << 1;   // 10 (Left Shift)
print flags >> 1;   // 2 (Right Shift)
```

---

## 5. Control Flow

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
The `switch` statement executes a block of code based on strict equality. Variables declared inside a `case` block are properly block-scoped and will not leak to other cases.

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

## 6. Loops

NARUTO supports both `while` and `for` loops. Loop blocks have their own isolated variable scopes.

### While Loop
```cpp
int clones = 0;
while (clones < 3) {
    print "Shadow Clone Jutsu!";
    clones++;
}
```

### For Loop
Variables declared in the `for` initializer (like `int i`) are isolated entirely to the loop.
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
```

---

## 7. Arrays

Arrays are homogeneous (they must contain elements of the same type) and declared using `[]`.

```cpp
int[] missions = [1, 2, 3];
print missions[0]; // Prints 1

// Updating array elements
missions[1] = 99;

// Arrays dynamically resize!
missions[3] = 100; // Adds to the end of the array

// Built-in Array Methods & Properties
string[] names = [];
names.push("Naruto");
names.push("Sasuke");
names.push("Sakura");

print names.length; // Prints 3
```

---

## 8. Functions

Functions must declare their return type and parameter types. Use `void` if the function does not return a value.

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

## 9. Object-Oriented Programming

NARUTO fully supports Classes, Structs, Access Modifiers, and Inheritance.

### Classes and Access Modifiers
Classes encapsulate data and behavior. By default, members are `public`. You can use the `private` keyword to restrict access. 
The constructor method must be strictly named `init()`.

```cpp
class Ninja {
    string name;
    private int chakra; // Cannot be accessed outside this class!

    function void init(string n, int c) {
        this.name = n;
        this.chakra = c;
    }

    function void display() {
        print "Ninja: " + this.name;
    }
}
```

### Inheritance (`extends` and `super`)
A class can inherit from another class using the `extends` keyword. When a subclass is instantiated, it can call the parent's constructor using `super(...)` and access parent methods via `super.method()`.

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
Structs are lightweight data containers. Unlike classes, they cannot have methods or inheritance, and they cannot be instantiated with the `new` keyword.

```cpp
struct Location {
    int x;
    int y;
}

// Instantiated exactly like a function call
Location konoha = Location(10, 20);
print konoha.x;
```
