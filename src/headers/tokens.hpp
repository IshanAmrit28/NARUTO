#ifndef __TOKENS_H
#define __TOKENS_H
#include <string>

enum type
{
  // --- KEYWORDS: DATATYPES ---
  TOKEN_BYTE_TYPE,   // keyword 'byte'
  TOKEN_SHORT_TYPE,  // keyword 'short'
  TOKEN_INT_TYPE,    // keyword 'int'
  TOKEN_LONG_TYPE,   // keyword 'long'
  TOKEN_FLOAT_TYPE,  // keyword 'float'
  TOKEN_DOUBLE_TYPE, // keyword 'double'
  TOKEN_CHAR_TYPE,   // keyword 'char'
  TOKEN_STRING_TYPE, // keyword 'string'
  TOKEN_BOOL_TYPE,   // keyword 'bool'
  TOKEN_VOID_TYPE,   // keyword 'void' (useful for functions)

  // --- LITERALS (The actual values) ---
  TOKEN_ID,             // myVariable
  TOKEN_INT_LITERAL,    // 123
  TOKEN_FLOAT_LITERAL,  // 3.14
  TOKEN_CHAR_LITERAL,   // 'a'
  TOKEN_STRING_LITERAL, // "Hello World"

  // --- ARITHMETIC OPERATORS ---
  TOKEN_PLUS,     // +
  TOKEN_MINUS,    // -
  TOKEN_ASTERISK, // *
  TOKEN_SLASH,    // /
  TOKEN_PERCENT,  // %

  // --- COMPARISON OPERATORS ---
  TOKEN_DOUBLE_EQUALS, // ==
  TOKEN_NOT_EQUALS,    // !=
  TOKEN_LESS_THAN,     // <
  TOKEN_LESS_EQUAL,    // <=
  TOKEN_GREATER_THAN,  // >
  TOKEN_GREATER_EQUAL, // >=

  // --- LOGICAL OPERATORS ---
  TOKEN_AND, // &&
  TOKEN_OR,  // ||
  TOKEN_NOT, // !

  // --- ASSIGNMENT OPERATORS ---
  TOKEN_EQUALS,          // =
  TOKEN_PLUS_EQUALS,     // +=
  TOKEN_MINUS_EQUALS,    // -=
  TOKEN_ASTERISK_EQUALS, // *=
  TOKEN_SLASH_EQUALS,    // /=
  TOKEN_PERCENT_EQUALS,  // %=

  // --- BITWISE OPERATORS ---
  TOKEN_BITWISE_AND,        // &
  TOKEN_BITWISE_OR,         // |
  TOKEN_BITWISE_XOR,        // ^
  TOKEN_BITWISE_NOT,        // ~
  TOKEN_LEFT_SHIFT,         // <<
  TOKEN_RIGHT_SHIFT,        // >>
  TOKEN_LEFT_SHIFT_EQUALS,  // <<=
  TOKEN_RIGHT_SHIFT_EQUALS, // >>=

  // --- SPECIAL CHARACTERS ---
  TOKEN_DOT,           // .
  TOKEN_SEMICOLON,     // ;
  TOKEN_OPEN_PAREN,    // (
  TOKEN_CLOSE_PAREN,   // )
  TOKEN_OPEN_BRACE,    // {
  TOKEN_CLOSE_BRACE,   // }
  TOKEN_OPEN_BRACKET,  // [
  TOKEN_CLOSE_BRACKET, // ]
  TOKEN_COMMA,         // ,
  TOKEN_COLON,         // :

  // --- KEYWORDS: CONTROL FLOW & OTHERS ---
  TOKEN_IF,
  TOKEN_ELSE,
  TOKEN_WHILE,
  TOKEN_FOR,
  TOKEN_RETURN,
  TOKEN_FUNCTION, // keyword 'fn' or 'function'
  TOKEN_CONST,    // keyword 'const'
  TOKEN_TRUE,     // keyword 'true'
  TOKEN_FALSE,    // keyword 'false'
  TOKEN_NULL,     // keyword 'null'
  TOKEN_PRINT,    // keyword 'print'
  TOKEN_INPUT,    // keyword 'input'
  TOKEN_BREAK,
  TOKEN_CONTINUE,

  TOKEN_EOF
};

struct Token
{
  enum type TYPE;
  std::string VALUE;
  int line;
};

#endif