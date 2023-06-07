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
    int tokIdx = 0;
    int datIdx = 0;

    bool val = adder();

    std::cout << val << std::endl;

    char *tok = adder.getTokens();
    char *dat = adder.getData();
    int *cnt = adder.getCounter();

    std::cout << "No of tokens: " << *(cnt) << std::endl;

    while (tok[tokIdx] != 0)
    {
        switch (tok[tokIdx])
        {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
        case 32:
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
        case 38:
        case 39:
        case 40:
        case 41:
            std::cout << (int)tok[tokIdx] << ':' << lookup_table[tok[tokIdx]] << std::endl;
            break;
        case 42:
        case 43:
        case 44:
            std::cout << (int)tok[tokIdx] << ':' << lookup_table[tok[tokIdx]] << '[';
            while (dat[datIdx] != '\0')
            {
                std::cout << dat[datIdx++];
            }
            datIdx++;
            std::cout << "]" << std::endl;
            break;
        default:
            std::cout << (int)tok[tokIdx] << ": UNK TOKEN!" << std::endl;
            break;
        }
        tokIdx++;
    }

    for (int i = 0; i < 20; i++)
    {
        if (isalnum(dat[i]))
        {
            std::cout << dat[i];
        }
        else
        {
            std::cout << (int)dat[i];
        }
    }
    return 0;
}