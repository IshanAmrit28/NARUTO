#ifndef __LEXER_H
#define __LEXER_H

#include "tokens.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <unordered_map>

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

class Lexer
{
public:
  Lexer(std::string sourceCode)
  {
    source = sourceCode;
    cursor = 0;
    size = sourceCode.size();
    current = sourceCode[cursor];
    lineNumber = 1;
    characterNumber = 1;

    // Init Keywords
    keywords["if"] = TOKEN_IF;
    keywords["else"] = TOKEN_ELSE;
    keywords["while"] = TOKEN_WHILE;
    keywords["for"] = TOKEN_FOR;
    keywords["return"] = TOKEN_RETURN;
    keywords["function"] = TOKEN_FUNCTION;
    keywords["const"] = TOKEN_CONST;
    keywords["true"] = TOKEN_TRUE;
    keywords["false"] = TOKEN_FALSE;
    keywords["null"] = TOKEN_NULL;
    keywords["print"] = TOKEN_PRINT;
    keywords["byte"] = TOKEN_BYTE_TYPE;
    keywords["int"] = TOKEN_INT_TYPE;
    keywords["float"] = TOKEN_FLOAT_TYPE;
    keywords["string"] = TOKEN_STRING_TYPE;
    keywords["bool"] = TOKEN_BOOL_TYPE;
    keywords["char"] = TOKEN_CHAR_TYPE;
    keywords["long"] = TOKEN_LONG_TYPE;
    keywords["short"] = TOKEN_SHORT_TYPE;
    keywords["double"] = TOKEN_DOUBLE_TYPE;
    keywords["void"] = TOKEN_VOID_TYPE;
  }

  // Helper to print specific error location
  void error(std::string message)
  {
    std::cout << "[Lexer Error] " << message
              << " at Line: " << lineNumber
              << ", Column: " << characterNumber << std::endl;
    exit(1);
  }

  char advance()
  {
    if (cursor < size)
    {
      char temp = current;
      cursor++;
      characterNumber++;
      current = (cursor < size) ? source[cursor] : '\0';
      return temp;
    }
    return '\0';
  }

  // Returns the NEXT character (lookahead)
  char peekNext()
  {
    if (cursor + 1 < size)
      return source[cursor + 1];
    return '\0';
  }

  void check()
  {
    while (current == ' ' || current == '\n' || current == '\t' || current == '\r')
    {
      if (current == '\n')
      {
        lineNumber++;
        characterNumber = 1;
      }
      advance();
    }
  }

  Token *createToken(enum type TYPE, std::string value)
  {
    Token *newToken = new Token();
    newToken->TYPE = TYPE;
    newToken->VALUE = value;
    newToken->line = lineNumber;
    return newToken;
  }

  Token *tokenizeID()
  {
    std::stringstream buffer;
    buffer << advance();

    while (isalnum(current) || current == '_')
    {
      buffer << advance();
    }

    std::string raw = buffer.str();
    Token *newToken = new Token();
    newToken->line = lineNumber;

    if (keywords.find(raw) != keywords.end())
      newToken->TYPE = keywords[raw];
    else
      newToken->TYPE = TOKEN_ID;

    newToken->VALUE = raw;
    return newToken;
  }

  Token *tokenizeNumber()
  {
    std::stringstream buffer;
    bool isFloat = false;

    while (isdigit(current))
      buffer << advance();

    // Check for float: dot followed by a digit
    if (current == '.' && isdigit(peekNext()))
    {
      isFloat = true;
      buffer << advance();
      while (isdigit(current))
        buffer << advance();
    }

    Token *newToken = new Token();
    newToken->TYPE = isFloat ? TOKEN_FLOAT_LITERAL : TOKEN_INT_LITERAL;
    newToken->VALUE = buffer.str();
    newToken->line = lineNumber;
    return newToken;
  }

  Token *tokenizeString()
  {
    advance(); // Skip opening "
    std::stringstream buffer;

    while (current != '"' && cursor < size)
    {
      if (current == '\n')
      {
        lineNumber++;
        characterNumber = 1;
      }
      buffer << advance();
    }

    if (current != '"')
      error("Unterminated string literal");

    advance(); // Skip closing "

    Token *newToken = new Token();
    newToken->TYPE = TOKEN_STRING_LITERAL;
    newToken->VALUE = buffer.str();
    newToken->line = lineNumber;
    return newToken;
  }

  Token *tokenizeChar()
  {
    advance(); // Skip '

    if (current == '\'')
      error("Empty character literal");

    std::string value(1, advance()); // Take char

    if (current != '\'')
      error("Expected closing single quote '");

    advance(); // Skip closing '

    Token *newToken = new Token();
    newToken->TYPE = TOKEN_CHAR_LITERAL;
    newToken->VALUE = value;
    newToken->line = lineNumber;
    return newToken;
  }

  std::vector<Token *> tokenize()
  {
    std::vector<Token *> tokens;

    while (cursor < size)
    {
      check();
      if (cursor >= size)
        break;

      if (isalpha(current) || current == '_')
      {
        tokens.push_back(tokenizeID());
        continue;
      }

      if (isdigit(current))
      {
        tokens.push_back(tokenizeNumber());
        continue;
      }

      if (current == '"')
      {
        tokens.push_back(tokenizeString());
        continue;
      }

      if (current == '\'')
      {
        tokens.push_back(tokenizeChar());
        continue;
      }

      switch (current)
      {
      case ';':
        tokens.push_back(createToken(TOKEN_SEMICOLON, ";"));
        advance();
        break;
      case '(':
        tokens.push_back(createToken(TOKEN_OPEN_PAREN, "("));
        advance();
        break;
      case ')':
        tokens.push_back(createToken(TOKEN_CLOSE_PAREN, ")"));
        advance();
        break;
      case '{':
        tokens.push_back(createToken(TOKEN_OPEN_BRACE, "{"));
        advance();
        break;
      case '}':
        tokens.push_back(createToken(TOKEN_CLOSE_BRACE, "}"));
        advance();
        break;
      case ',':
        tokens.push_back(createToken(TOKEN_COMMA, ","));
        advance();
        break;
      case '*':
        tokens.push_back(createToken(TOKEN_ASTERISK, "*"));
        advance();
        break;
      case '%':
        tokens.push_back(createToken(TOKEN_PERCENT, "%"));
        advance();
        break;

      case '=':
        // FIX: Use peekNext() to safely check the NEXT char
        if (peekNext() == '=')
        {
          advance();
          advance();
          tokens.push_back(createToken(TOKEN_DOUBLE_EQUALS, "=="));
        }
        else
        {
          advance();
          tokens.push_back(createToken(TOKEN_EQUALS, "="));
        }
        break;

      case '!':
        if (peekNext() == '=')
        {
          advance();
          advance();
          tokens.push_back(createToken(TOKEN_NOT_EQUALS, "!="));
        }
        else
        {
          advance();
          tokens.push_back(createToken(TOKEN_NOT, "!"));
        }
        break;

      case '<':
        if (peekNext() == '=')
        {
          advance();
          advance();
          tokens.push_back(createToken(TOKEN_LESS_EQUAL, "<="));
        }
        else
        {
          advance();
          tokens.push_back(createToken(TOKEN_LESS_THAN, "<"));
        }
        break;

      case '>':
        if (peekNext() == '=')
        {
          advance();
          advance();
          tokens.push_back(createToken(TOKEN_GREATER_EQUAL, ">="));
        }
        else
        {
          advance();
          tokens.push_back(createToken(TOKEN_GREATER_THAN, ">"));
        }
        break;

      case '+':
        if (peekNext() == '=')
        {
          advance();
          advance();
          tokens.push_back(createToken(TOKEN_PLUS_EQUALS, "+="));
        }
        else
        {
          advance();
          tokens.push_back(createToken(TOKEN_PLUS, "+"));
        }
        break;

      case '-':
        if (peekNext() == '=')
        {
          advance();
          advance();
          tokens.push_back(createToken(TOKEN_MINUS_EQUALS, "-="));
        }
        else
        {
          advance();
          tokens.push_back(createToken(TOKEN_MINUS, "-"));
        }
        break;

      case '/':
        if (peekNext() == '/')
        {
          while (current != '\n' && cursor < size)
            advance();
        }
        else
        {
          tokens.push_back(createToken(TOKEN_SLASH, "/"));
          advance();
        }
        break;

      default:
        std::cout << "[Lexer Error] Unknown character: '" << current << "'"
                  << " at Line: " << lineNumber
                  << ", Column: " << characterNumber << std::endl;
        exit(1);
      }
    }

    tokens.push_back(createToken(TOKEN_EOF, "EOF"));
    return tokens;
  }

private:
  std::string source;
  int cursor;
  int size;
  char current;
  int lineNumber;
  int characterNumber;
  std::unordered_map<std::string, type> keywords;
};

#endif