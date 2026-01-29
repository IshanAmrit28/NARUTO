#ifndef __LEXER_H
#define __LEXER_H

#include "tokens.hpp"

#include <string>
#include <cctype>
#include <vector>
#include <sstream>
#include <iostream>
#include <unordered_map>

std::string typeToString(enum type TYPE)
{
  switch (TYPE)
  {
  case TOKEN_ID:
    return "TOKEN_ID";
  case TOKEN_INT:
    return "TOKEN_INT";
  case TOKEN_EQUALS:
    return "TOKEN_EQUALS";
  case TOKEN_SEMICOLON:
    return "TOKEN_SEMICOLON";
  case TOKEN_OPEN_PAREN:
    return "TOKEN_OPEN_PAREN";
  case TOKEN_CLOSE_PAREN:
    return "TOKEN_CLOSE_PAREN";
  case TOKEN_CLOSE_BRACE:
    return "TOKEN_CLOSE_BRACE";
  case TOKEN_OPEN_BRACE:
    return "TOKEN_OPEN_BRACE";
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
    else
    {
      return '\0';
    }
  }

  void check()
  {
    while (current == ' ' || current == '\n' || current == '\t' || current == '\r')
    {
      if (current == '\n')
      {
        lineNumber++;
        characterNumber = 0;
      }
      advance();
    }
  }

  Token *tokenizeID()
  {
    std::stringstream buffer;
    buffer << advance();
    while (isalnum(current) || current == '_')
    {
      buffer << advance();
    }
    Token *newToken = new Token();
    newToken->TYPE = TOKEN_ID;
    newToken->VALUE = buffer.str();

    return newToken;
  }

  Token *tokenizeINT()
  {
    std::stringstream buffer;
    buffer << advance();
    while (isdigit(current))
    {
      buffer << advance();
    }
    Token *newToken = new Token();
    newToken->TYPE = TOKEN_INT,
    newToken->VALUE = buffer.str();

    return newToken;
  }

  Token *tokenizeSPECIAL(enum type TYPE)
  {
    Token *newToken = new Token();
    newToken->TYPE = TYPE;
    newToken->VALUE = std::string(1, advance());

    return newToken;
  }

  std::vector<Token *> tokenize()
  {
    std::vector<Token *> tokens;

    bool notEOF = true;
    while (cursor < size && notEOF)
    {
      check();
      if (isalpha(current) || current == '_') // logic for ids
      {
        tokens.push_back(tokenizeID());
        continue;
      }

      if (isdigit(current)) // for integers
      {
        tokens.push_back(tokenizeINT());
        continue;
      }

      switch (current)
      {
      case ';':
      {
        tokens.push_back(tokenizeSPECIAL(TOKEN_SEMICOLON));
        break;
      }
      case '(':
      {
        tokens.push_back(tokenizeSPECIAL(TOKEN_OPEN_PAREN));
        break;
      }
      case ')':
      {
        tokens.push_back(tokenizeSPECIAL(TOKEN_CLOSE_PAREN));
        break;
      }
      case '{':
      {
        tokens.push_back(tokenizeSPECIAL(TOKEN_OPEN_BRACE));
        break;
      }
      case '}':
      {
        tokens.push_back(tokenizeSPECIAL(TOKEN_CLOSE_BRACE));
        break;
      }
      case '=':
      {
        tokens.push_back(tokenizeSPECIAL(TOKEN_EQUALS));
        break;
      }
      default:
      {
        std::cout << "unknown character: " << current << std::endl;
        std::cout << "at line: " << lineNumber << " character: " << characterNumber << std::endl;
        exit(1);
      }
      }
    }
    return tokens;
  }

  char peak(int offset = 0)
  {
    if (cursor + offset < size)
    {
      return source[cursor + offset];
    }
    else
    {
      return '\0';
    }
  }

private:
  std::string source;
  int cursor;
  int size;
  char current;
  int lineNumber;
  int characterNumber;
};

#endif