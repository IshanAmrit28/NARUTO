#ifndef __LEXER_H
#define __LEXER_H

#include "tokens.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <unordered_map>

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
        else if (peekNext() == '<')
        {
          advance();
          advance();
          tokens.push_back(createToken(TOKEN_LEFT_SHIFT, "<<"));
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
        else if (peekNext() == '>')
        {
          advance();
          advance();
          tokens.push_back(createToken(TOKEN_RIGHT_SHIFT, ">>"));
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
        else if (peekNext() == '+')
        {
          advance();
          advance();
          tokens.push_back(createToken(TOKEN_INCREMENT, "++"));
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
        else if (peekNext() == '-')
        {
          advance();
          advance();
          tokens.push_back(createToken(TOKEN_DECREMENT, "--"));
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
      case '.':
        tokens.push_back(createToken(TOKEN_DOT, "."));
        advance();
        break;

      case ':':
        tokens.push_back(createToken(TOKEN_COLON, ":"));
        advance();
        break;
      case '&':
        if (peekNext() == '&')
        {
          advance();
          advance();
          tokens.push_back(createToken(TOKEN_AND, "&&"));
        }
        else
        {
          tokens.push_back(createToken(TOKEN_BITWISE_AND, "&"));
          advance();
        }
        break;
      case '|':
        if (peekNext() == '|')
        {
          advance();
          advance();
          tokens.push_back(createToken(TOKEN_OR, "||"));
        }
        else
        {
          tokens.push_back(createToken(TOKEN_BITWISE_OR, "|"));
          advance();
        }
        break;
      case '^':
        tokens.push_back(createToken(TOKEN_BITWISE_XOR, "^"));
        advance();
        break;
      case '~':
        tokens.push_back(createToken(TOKEN_BITWISE_NOT, "~"));
        advance();
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