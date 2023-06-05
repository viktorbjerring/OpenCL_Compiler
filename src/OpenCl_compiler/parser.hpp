#include <iostream>

#include "../openCL/context.hpp"
#include "../openCL/program.hpp"

#ifndef CEBC928B_1EE5_4152_AAB7_A0E570249CA8
#define CEBC928B_1EE5_4152_AAB7_A0E570249CA8

class parser {
  private:
    open_cl::Context _context;
    open_cl::Program _program;
    
    cl::Kernel _adder;
    
    int* _data;
    
  public:
    parser() = default;
    parser(open_cl::Context& context) {
        setContext(context);
        _data = new int;
    }
    
    inline void setContext(open_cl::Context& context) {
        _context = context;
        _program = open_cl::Program(_context);
        _program.open("parser.cl");
        _adder = _program("parser");
    }
    
    int* getData() const {
        return _data;
    }
    
    bool operator()() {
        cl::Buffer mem_buffer(_context(), CL_MEM_WRITE_ONLY, sizeof(int));
        int a = 20;
        int b = 30;
        _adder.setArg(0, a);
        _adder.setArg(1, b);
        _adder.setArg(2, mem_buffer);
        auto ret = _context.getContextQueue().enqueueNDRangeKernel(_adder, cl::NullRange, cl::NDRange(1));
        
        if(ret) {
            std::cout << "ndrange: " << ret << std::endl;
            return false;
        }
        
        ret = _context.getContextQueue().enqueueReadBuffer(mem_buffer, CL_TRUE, 0, sizeof(int), _data);
        
        if(ret) {
            std::cout << "read: " << ret << std::endl;
            return false;
        }
        
        ret = _context.getContextQueue().finish();
        
        if(ret) {
            std::cout << "finish: " << ret << std::endl;
            return false;
        }
        
        return true;;
    }
};

#endif /* CEBC928B_1EE5_4152_AAB7_A0E570249CA8 */
