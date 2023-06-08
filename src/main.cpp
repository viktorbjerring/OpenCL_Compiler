#include <iostream>
#include <fstream>
#include <chrono>
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
    if (argc < 3)
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

    auto start = std::chrono::high_resolution_clock::now();

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

    auto readfile = std::chrono::high_resolution_clock::now();

    // Read input
    char *c_code = new char[inlength];
    codefile.read(c_code, inlength);

    auto createkernel = std::chrono::high_resolution_clock::now();

    // Create context and kernel
    auto context = open_cl::Context::autoGenerate();
    auto _parser = parser(context);

    // Copy string to kernel buffer
    _parser.setInput(c_code, strlen(c_code));

    auto prekernel = std::chrono::high_resolution_clock::now();

    // Run kernel
    bool val = _parser();

    auto postkernel = std::chrono::high_resolution_clock::now();

    if (!val)
    {
        std::cout << "Error with kernel :(" << std::endl;
        return -1;
    }

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

    // Get data from kernel
    char *tok = _parser.getTokens();
    char *dat = _parser.getData();

    auto readkernel = std::chrono::high_resolution_clock::now();

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
    auto writefile = std::chrono::high_resolution_clock::now();

    std::cout << "Time to open file: " << std::chrono::duration_cast<std::chrono::microseconds>(readfile - start).count() << std::endl;
    std::cout << "Time to read input: " << std::chrono::duration_cast<std::chrono::microseconds>(createkernel - readfile).count() << std::endl;
    std::cout << "Time to create kernel: " << std::chrono::duration_cast<std::chrono::microseconds>(prekernel - createkernel).count() << std::endl;
    std::cout << "Time to run kernel: " << std::chrono::duration_cast<std::chrono::microseconds>(postkernel - prekernel).count() << std::endl;
    std::cout << "Time to read kernel: " << std::chrono::duration_cast<std::chrono::microseconds>(readkernel - postkernel).count() << std::endl;
    std::cout << "Time to write file: " << std::chrono::duration_cast<std::chrono::microseconds>(writefile - readkernel).count() << std::endl;
    std::cout << "Time to execute: " << std::chrono::duration_cast<std::chrono::microseconds>(writefile - start).count() << std::endl;
    return 0;
}