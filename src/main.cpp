#include <iostream>
#include <fstream>
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
    // Check no of args
    if (argc != 3)
    {
        std::cout << "Please enter valid args.\nUsage: [inputfile] [outputfile]" << std::endl;
        return -1;
    }

    // Open input
    std::ifstream codefile(argv[1]);
    if (!codefile.is_open())
    {
        std::cout << "Error in opening input file" << std::endl;
        return -1;
    }

    // Open output
    std::ofstream lexfile(argv[2]);
    if (!lexfile.is_open())
    {
        std::cout << "Error in opening output file" << std::endl;
        return -1;
    }

    // Length of input
    codefile.seekg(0, codefile.end);
    int inlength = codefile.tellg();
    codefile.seekg(0, codefile.beg);

    // Check length is ok
    if (inlength >= STRING_LENGTH)
    {
        std::cout << "Input too long :(\nMax length: " << STRING_LENGTH << "\nCurrent length: " << inlength << std::endl;
        return -1;
    }

    // Read input
    char *c_code = new char[inlength];
    codefile.read(c_code, inlength);

    // Create context and kernel
    auto context = open_cl::Context::autoGenerate();
    auto _parser = parser(context);

    // Copy string to kernel buffer
    _parser.setInput(c_code, strlen(c_code));

    // Run kernel
    bool val = _parser();

    if (!val)
    {
        std::cout << "Error with kernel :(" << std::endl;
        return -1;
    }

    // Get data from kernel
    char *tok = _parser.getTokens();
    char *dat = _parser.getData();
    int *ret = _parser.getRetVal();

    if (*(ret) == -1)
    {
        std::cout << "No of tokens exceed buffer size :(" << std::endl;
        return -1;
    }
    else if (*(ret) == -2)
    {
        std::cout << "No of data exceed buffer size :(" << std::endl;
        return -1;
    }

    int tokIdx = 0;
    int datIdx = 0;

    // Printing loop
    while (tok[tokIdx] != 0)
    {
        if (tok[tokIdx] >= 1 && tok[tokIdx] <= 44)
        {
            lexfile << tokIdx << ':' << lookup_table[tok[tokIdx]];
            if (tok[tokIdx] >= 42)
            {
                lexfile << "[";
                while (dat[datIdx] != '\0')
                {
                    lexfile << dat[datIdx++];
                }
                datIdx++;
                lexfile << "]";
            }
        }
        else
        {
            lexfile << "UNK TOKEN: " << (int)tok[tokIdx];
        }
        lexfile << std::endl;
        tokIdx++;
    }
    return 0;
}