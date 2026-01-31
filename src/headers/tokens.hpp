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
                 // TOKEN_POWER,  // **  //future updates

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
  TOKEN_INCREMENT,       // ++
  TOKEN_DECREMENT,       // --
                   // TOKEN_POWER_EQUALS, // **= //future updates

  // --- BITWISE OPERATORS ---
  TOKEN_BITWISE_AND, // &
  TOKEN_BITWISE_OR,  // |
  TOKEN_BITWISE_XOR, // ^
  TOKEN_BITWISE_NOT, // ~
  TOKEN_LEFT_SHIFT,  // <<
  TOKEN_RIGHT_SHIFT, // >>
                     // TOKEN_LEFT_SHIFT_EQUALS,  // <<= future updates
                     // TOKEN_RIGHT_SHIFT_EQUALS, // >>= future updates

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

// Helper to convert Token Type to String

std::string typeToString(enum type TYPE)
{
  switch (TYPE)
  {
  case TOKEN_BYTE_TYPE:
    return "TOKEN_BYTE_TYPE";
  case TOKEN_INT_TYPE:
    return "TOKEN_INT_TYPE";
  case TOKEN_FLOAT_TYPE:
    return "TOKEN_FLOAT_TYPE";
  case TOKEN_STRING_TYPE:
    return "TOKEN_STRING_TYPE";
  case TOKEN_CHAR_TYPE:
    return "TOKEN_CHAR_TYPE";
  case TOKEN_BOOL_TYPE:
    return "TOKEN_BOOL_TYPE";
  case TOKEN_LONG_TYPE:
    return "TOKEN_LONG_TYPE";
  case TOKEN_SHORT_TYPE:
    return "TOKEN_SHORT_TYPE";
  case TOKEN_DOUBLE_TYPE:
    return "TOKEN_DOUBLE_TYPE";
  case TOKEN_VOID_TYPE:
    return "TOKEN_VOID_TYPE";

  case TOKEN_TRUE:
    return "TOKEN_TRUE";
  case TOKEN_FALSE:
    return "TOKEN_FALSE";
  case TOKEN_NULL:
    return "TOKEN_NULL";
  case TOKEN_IF:
    return "TOKEN_IF";
  case TOKEN_ELSE:
    return "TOKEN_ELSE";
  case TOKEN_WHILE:
    return "TOKEN_WHILE";
  case TOKEN_FOR:
    return "TOKEN_FOR";
  case TOKEN_PRINT:
    return "TOKEN_PRINT";
  case TOKEN_RETURN:
    return "TOKEN_RETURN";

  case TOKEN_ID:
    return "TOKEN_ID";
  case TOKEN_INT_LITERAL:
    return "TOKEN_INT_LITERAL";
  case TOKEN_FLOAT_LITERAL:
    return "TOKEN_FLOAT_LITERAL";
  case TOKEN_STRING_LITERAL:
    return "TOKEN_STRING_LITERAL";
  case TOKEN_CHAR_LITERAL:
    return "TOKEN_CHAR_LITERAL";

  case TOKEN_PLUS:
    return "TOKEN_PLUS";
  case TOKEN_MINUS:
    return "TOKEN_MINUS";
  case TOKEN_ASTERISK:
    return "TOKEN_ASTERISK";
  case TOKEN_SLASH:
    return "TOKEN_SLASH";
  case TOKEN_EQUALS:
    return "TOKEN_EQUALS";
  case TOKEN_DOUBLE_EQUALS:
    return "TOKEN_DOUBLE_EQUALS";

  case TOKEN_SEMICOLON:
    return "TOKEN_SEMICOLON";
  case TOKEN_OPEN_PAREN:
    return "TOKEN_OPEN_PAREN";
  case TOKEN_CLOSE_PAREN:
    return "TOKEN_CLOSE_PAREN";
  case TOKEN_OPEN_BRACE:
    return "TOKEN_OPEN_BRACE";
  case TOKEN_CLOSE_BRACE:
    return "TOKEN_CLOSE_BRACE";
  case TOKEN_COMMA:
    return "TOKEN_COMMA";
  case TOKEN_EOF:
    return "TOKEN_EOF";

  default:
    return "UNKNOWN_TOKEN";
  }
}
struct Token
{
  enum type TYPE;
  std::string VALUE;
  int line;
};

#endif