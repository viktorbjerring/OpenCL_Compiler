#include <iostream>
#include "openCL/context.hpp"
#include "OpenCl_compiler/parser.hpp"

const char *const lookup_table[45] = {
    "\0",
    "Comma",
    "Semicolon",
    "Lparen",
    "Rparen",
    "Lbrack",
    "Rbrack",
    "Lbrace",
    "Rbrace",
    "Dot",
    "Plus",
    "Minus",
    "Divide",
    "Times",
    "Eq",
    "And",
    "Or",
    "Caret",
    "Gt",
    "Ge",
    "Lt",
    "Neq",
    "Le",
    "Colon",
    "Assign",
    "Array",
    "Break",
    "Do",
    "Else",
    "End",
    "For",
    "Function",
    "In",
    "If",
    "Let",
    "Nil",
    "Of",
    "Then",
    "To",
    "Type",
    "Var",
    "While",
    "Int",
    "String",
    "ID"};

int main(int argc, char *argv[])
{

    auto context = open_cl::Context::autoGenerate();

    auto adder = parser(context);
    bool val = adder();

    std::cout << val << std::endl;

    char *ret = adder.getTokens();
    char *pos = adder.getData();
    int i = 0;

    while (ret[i] != 0)
    {
        if (ret[i] < 45)
        {
            std::cout << lookup_table[ret[i]] << ':' << (int)ret[i] << ':' << (int)pos[i] << std::endl;
        }
        else
        {
            std::cout << (int)ret[i] << ':' << (int)pos[i] << std::endl;
        }
        i++;
    }
    return 0;
}