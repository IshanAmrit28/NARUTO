# 📜 Naruto Lang - Syntax Documentation v1.0

## 1. Program Structure

Your language is **script-based**. You do not need a `class Main` or `void main()`. The compiler executes code from the top of the file to the bottom.

### Comments

Use `//` for single-line comments. The lexer ignores everything after `//` until the next line.

```c
// This is a comment
int x = 10; // Variable declaration
2. Data Types & Literals
Your language is Statically Typed. You must declare the type of every variable.

Type	Keyword	Example Literal	Description
Integer	int	10, -500	32-bit whole number
Float	float	3.14, 0.5	32-bit decimal number
Double	double	99.99999	64-bit decimal number
String	string	"Hello"	Text enclosed in double quotes
Boolean	bool	true, false	Logical true or false
Character	char	'A', 'z'	Single character in single quotes
Byte	byte	255	8-bit unsigned integer
Short	short	32000	16-bit integer
Long	long	123456789	64-bit integer
Void	void	null	Used for functions returning nothing
3. Variables & Constants
Variable Declaration
Syntax: Type identifier = value;

int age = 20;
string name = "Naruto";
float chakra = 95.5;
Constants
Use the const keyword to declare variables that cannot change.

const float PI = 3.14159;
const int MAX_USERS = 100;
4. Input & Output
Print (Output)
Use the print keyword to output data to the console. It supports expressions.

print "Hello World";
print 10 + 20;
print age;
Input
Use the input keyword to get data from the user. It supports an optional prompt string inside parentheses.

Behavior:

input() reads a full line of text.

The compiler automatically attempts to convert the input to the target variable's type.

// 1. Basic String Input
string name = input("Enter your name: ");

// 2. Integer Input (Auto-converted)
int age = input("Enter your age: ");

// 3. Float Input
float height = input("Enter height: ");
Note: If the user types "abc" for an int variable, the runtime will throw a conversion error.

5. Arrays
Arrays are fixed-size collections of a single data type.

Declaration
Use square brackets [] for both the type definition and the array literal.

int[] numbers = [10, 20, 30, 40, 50];
string[] team7 = ["Naruto", "Sasuke", "Sakura"];
Accessing & Modifying
Use zero-based indexing.

print numbers[0];  // Prints 10
numbers[1] = 99;   // Changes 20 to 99
6. Operators
Arithmetic
int sum = 10 + 5;
int diff = 10 - 2;
int prod = 10 * 2;
int quot = 10 / 2;
int rem = 10 % 3;
Increment / Decrement
x++;
y--;
Comparison
bool isEqual = (a == b);
bool notEqual = (a != b);
bool greater = (a > b);
bool check = (a >= 10);
Logical
if (age > 18 && hasID == true) { }
if (score < 50 || isCheating) { }
if (!isValid) { }
Bitwise
Supported bitwise operations:

& AND

| OR

^ XOR

~ NOT

<< Left Shift

>> Right Shift

7. Control Flow
If / Else
if (chakra < 10) {
    print "Warning: Low Chakra";
} else if (chakra > 100) {
    print "Overload!";
} else {
    print "Normal levels";
}
While Loop
int count = 5;
while (count > 0) {
    print count;
    count--;
}
For Loop
for (int i = 0; i < 10; i++) {
    print i;
}
Break & Continue
break exits the loop

continue skips to the next iteration

8. Functions
Functions are declared using the function keyword.

Syntax
function ReturnType FunctionName(Parameters) { ... }
Examples
function int add(int a, int b) {
    return a + b;
}

function void greet(string name) {
    print "Hello " + name;
}

int result = add(5, 10);
greet("Kakashi");
9. Example Program
// Global Constants
const float PI = 3.14;
string[] users = ["Alice", "Bob"];

// Function Definition
function int calculateArea(int radius) {
    return PI * radius * radius;
}

// Main Script
print "Welcome to the System";

string user = input("Who are you? ");
int radius = input("Enter radius: ");

if (radius > 0) {
    int area = calculateArea(radius);
    print "The area is: ";
    print area;
} else {
    print "Invalid radius!";
}

print "Counting down:";
for (int i = 5; i > 0; i--) {
    print i;
}
```
