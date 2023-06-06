#include <iostream>
#include"openCL/context.hpp"
#include "OpenCl_compiler/parser.hpp"

int main(int argc, char *argv[]) {

    auto context = open_cl::Context::autoGenerate();

    auto adder = parser(context);
    bool val = adder();

    std::cout << val << std::endl;

    char* ret = adder.getData();

    for (int i = 0; i < 10; i++) {
        std::cout << (int)ret[i] << ':';
    }

    std::cout << "value: "  << std::endl;
    return 0;
}