#include <iostream>
#include"Compiler.hpp"

int main(int argc, char *argv[]) {
    Lexer lex;

    std::ifstream ifs;
    ifs.open(argv[1]);

    lex.ReadTokens(ifs);

    lex.WriteTokens();

    return 0;
}