#include <iostream>
#include"openCL/context.hpp"
#include "OpenCl_compiler/parser.hpp"

int main(int argc, char *argv[]) {

    auto context = open_cl::Context::autoGenerate();

    auto adder = parser(context);
    bool val = adder();

    std::cout << val << std::endl;

    int ret = *(adder.getData());

    std::cout << "value: " << ret << std::endl;
    return 0;
}