#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "headers/lexer.hpp"
#include "headers/ast.hpp"
#include "headers/parser.hpp"
#include "headers/type_checker.hpp"
#include "headers/interpreter.hpp"

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cout << "Usage: naruto <file.nt>";
    exit(1);
  }
  std::ifstream sourceFileStream(argv[1]);
  if (!sourceFileStream.is_open())
  {
    std::cerr << "Could not open file: " << argv[1] << std::endl;
    exit(1);
  }

  std::stringstream buffer;
  char temp;
  while (sourceFileStream.get(temp))
    buffer << temp;
  std::string sourceCode = buffer.str();

  // 1. LEXER
  Lexer lexer(sourceCode);
  std::vector<Token *> tokens = lexer.tokenize();

  // 2. PARSER
  PARSER parser(tokens);
  std::vector<STATEMENT *> programAST = parser.generate_ast();

  // 3. TYPE CHECKER
  TYPE_CHECKER typeChecker;
  typeChecker.analyze(programAST);

  // 4. INTERPRETER (The Runtime)
  std::cout << "\n--- PROGRAM OUTPUT ---\n";

  INTERPRETER interpreter;
  interpreter.execute(programAST);

  return 0;
}