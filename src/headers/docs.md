Here is the complete, cohesive syntax documentation for **Naruto Lang v1.0** in Markdown format. You can copy-paste this directly into a `README.md` or `SYNTAX.md` file.

````markdown
# 📜 Naruto Lang - Syntax Documentation v1.0

## 1. Program Structure

Your language is **script-based**. You do not need a `class Main` or `void main()`. The compiler executes code from the top of the file to the bottom.

**Comments:**
Use `//` for single-line comments. The lexer ignores everything after `//` until the next line.

```c
// This is a comment
int x = 10; // Variable declaration
```
````

---

## 2. Data Types & Literals

Your language is **Statically Typed**. You must declare the type of every variable.

| Type          | Keyword  | Example Literal | Description                          |
| ------------- | -------- | --------------- | ------------------------------------ |
| **Integer**   | `int`    | `10`, `-500`    | 32-bit whole number                  |
| **Float**     | `float`  | `3.14`, `0.5`   | 32-bit decimal number                |
| **Double**    | `double` | `99.99999`      | 64-bit decimal number                |
| **String**    | `string` | `"Hello"`       | Text enclosed in double quotes       |
| **Boolean**   | `bool`   | `true`, `false` | Logical true or false                |
| **Character** | `char`   | `'A'`, `'z'`    | Single character in single quotes    |
| **Byte**      | `byte`   | `255`           | 8-bit unsigned integer               |
| **Short**     | `short`  | `32000`         | 16-bit integer                       |
| **Long**      | `long`   | `123456789`     | 64-bit integer                       |
| **Void**      | `void`   | `null`          | Used for functions returning nothing |

---

## 3. Variables & Constants

### Variable Declaration

Syntax: `Type identifier = value;`

```c
int age = 20;
string name = "Naruto";
float chakra = 95.5;

```

### Constants

Use the `const` keyword to declare variables that cannot change.

```c
const float PI = 3.14159;
const int MAX_USERS = 100;

```

---

## 4. Input & Output

### Print (Output)

Use the `print` keyword to output data to the console. It supports expressions.

```c
print "Hello World";
print 10 + 20;
print age;

```

### Input

Use the `input` keyword to get data from the user. It supports an optional **prompt string** inside parentheses.

**Behavior:**

- `input()` reads a full line of text.
- The compiler automatically attempts to convert the input to the target variable's type.

```c
// 1. Basic String Input
string name = input("Enter your name: ");

// 2. Integer Input (Auto-converted)
int age = input("Enter your age: ");

// 3. Float Input
float height = input("Enter height: ");

```

_Note: If the user types "abc" for an `int` variable, the runtime will throw a conversion error._

---

## 5. Arrays

Arrays are fixed-size collections of a single data type.

### Declaration

Use square brackets `[]` for both the type definition and the array literal.

```c
// Syntax: Type[] name = [v1, v2, v3];

int[] numbers = [10, 20, 30, 40, 50];
string[] team7 = ["Naruto", "Sasuke", "Sakura"];

```

### Accessing & Modifying

Use zero-based indexing.

```c
print numbers[0];  // Prints 10

numbers[1] = 99;   // Changes 20 to 99

```

---

## 6. Operators

### Arithmetic

Standard math operations.

```c
int sum = 10 + 5;
int diff = 10 - 2;
int prod = 10 * 2;
int quot = 10 / 2;
int rem = 10 % 3;

```

### Increment/Decrement

Shorthand operators for increasing or decreasing values by 1.

```c
x++; // Increases x by 1
y--; // Decreases y by 1

```

### Comparison

Returns `true` or `false`.

```c
bool isEqual = (a == b);
bool notEqual = (a != b);
bool greater = (a > b);
bool check = (a >= 10);

```

### Logical

Used to combine booleans.

```c
if (age > 18 && hasID == true) { ... } // AND
if (score < 50 || isCheating) { ... }  // OR
if (!isValid) { ... }                  // NOT

```

### Bitwise

Supported bitwise operations for low-level manipulation.

- `&` (AND), `|` (OR), `^` (XOR), `~` (NOT)
- `<<` (Left Shift), `>>` (Right Shift)

---

## 7. Control Flow

### If / Else

Standard syntax using parentheses `()` and braces `{}`.

```c
if (chakra < 10) {
    print "Warning: Low Chakra";
} else if (chakra > 100) {
    print "Overload!";
} else {
    print "Normal levels";
}

```

### While Loop

Runs while the condition is true.

```c
int count = 5;
while (count > 0) {
    print count;
    count--;
}

```

### For Loop

Standard C-style loop structure.

```c
// Syntax: for (init; condition; update)
for (int i = 0; i < 10; i++) {
    print i;
}

```

### Break & Continue

- `break`: Exits the loop immediately.
- `continue`: Skips to the next iteration.

---

## 8. Functions

Functions are declared using the `function` keyword. They must specify a return type (or `void` if they return nothing).

### Syntax

`function ReturnType FunctionName(Parameters) { ... }`

### Examples

```c
// 1. Function that returns a value
function int add(int a, int b) {
    return a + b;
}

// 2. Function that returns nothing (void)
function void greet(string name) {
    print "Hello " + name; // String concatenation
}

// 3. Calling functions
int result = add(5, 10);
greet("Kakashi");

```

---

## 9. Example Program

Here is a complete program demonstrating the language syntax.

```c
// 1. Global Variables
const float PI = 3.14;
string[] users = ["Alice", "Bob"];

// 2. Function Definition
function int calculateArea(int radius) {
    return PI * radius * radius;
}

// 3. Main Script
print "Welcome to the System";

// Input
string user = input("Who are you? ");
int radius = input("Enter radius: ");

// Logic
if (radius > 0) {
    int area = calculateArea(radius);
    print "The area is: ";
    print area;
} else {
    print "Invalid radius!";
}

// Loops
print "Counting down:";
for (int i = 5; i > 0; i--) {
    print i;
}

```

```

```
