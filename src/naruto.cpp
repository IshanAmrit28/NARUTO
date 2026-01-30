#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "headers/lexer.hpp"

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cout << "please suplly source file";
    exit(1);
  }
  // naruto example.nt
  std::cout << "processing file: " << argv[1] << std::endl;
  std::ifstream sourceFileStream(argv[1]);

  std::stringstream buffer;

  char temp;
  while (sourceFileStream.get(temp))
  {
    buffer << temp;
  }
  std::string sourceCode = buffer.str();
  std::cout << "file content:\n"
            << sourceCode << std::endl;

  Lexer lexer(sourceCode);
  std::vector<Token *> tokens = lexer.tokenize();
  int counter = 0;
  std::cout << "Tokens: " << std::endl;
  for (Token *t : tokens)
  {
    counter++;
    std::cout << counter << ") " << "Type: " << typeToString(t->TYPE) << " Value: " << t->VALUE << std::endl;
    }
  std::cout << "END OF PEROGRAM" << std::endl;

  return 0;
}