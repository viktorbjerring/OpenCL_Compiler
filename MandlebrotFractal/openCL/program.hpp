#include "context.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <stdarg.h>
#include <map>

#ifndef E3B190A8_9B13_4CD5_8DE4_68B62FD3F10F
#define E3B190A8_9B13_4CD5_8DE4_68B62FD3F10F

namespace open_cl {
    class Program {
      private:
        cl::Program* _program = nullptr;
        Context _context;
      public:
        // Rule of six because c++ likes you to add a ton of code :)
        Program() = default;
        Program(Context& context);
        ~Program();
        Program(const Program& toCopy);
        Program(Program&& toMove);
        const Program& operator=(const Program& toCopy);
        const Program& operator=(Program&& toMove);
        
        void setContext(Context& context);
        
        bool open(std::string path);
        bool openBinary(std::string path);
        
        cl::Program& operator()() const;
        
        Context getContext() const;
        
        cl::Kernel operator()(std::string kernelName) const;
    };
}

#endif /* E3B190A8_9B13_4CD5_8DE4_68B62FD3F10F */
