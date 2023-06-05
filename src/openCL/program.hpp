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
        Program(Context& context) {
            _context = context;
        }
        ~Program() {
            if(_program != nullptr) {
                delete _program;
            }
        }
        Program(const Program& toCopy) {
            _context = toCopy._context;
            
            if(toCopy._program != nullptr) {
                _program = new cl::Program();
                *_program = *toCopy._program;
            }
        }
        Program(Program&& toMove) {
            _context = toMove._context;
            
            if(toMove._program != nullptr) {
                _program = toMove._program;
                toMove._program = nullptr;
            }
        }
        const Program& operator=(const Program& toCopy) {
            _context = toCopy._context;
            
            if(toCopy._program != nullptr) {
                _program = new cl::Program();
                *_program = *toCopy._program;
            }
            
            return *this;
        }
        const Program& operator=(Program&& toMove) {
            _context = toMove._context;
            
            if(toMove._program != nullptr) {
                _program = toMove._program;
                toMove._program = nullptr;
            }
            
            return *this;
        }
        
        void setContext(Context& context) {
            _context = context;
        }
        
        bool open(std::string path) {
            std::ifstream ifs(path);
            
            if(!ifs.is_open()) {
                std::cerr << "Failed to load kernel.\n";
                return false;
            }
            
            std::string kernel_code((std::istreambuf_iterator<char>(ifs)),
                                    (std::istreambuf_iterator<char>()));
            cl::Program::Sources sources;
            sources.push_back({kernel_code.c_str(), kernel_code.length()});
            _program = new cl::Program(_context(), sources);
            
            if(_program->build({_context.getDevice()}) != CL_SUCCESS) {
                std::cerr << "Error building: " << _program->getBuildInfo<CL_PROGRAM_BUILD_LOG>(_context.getDevice()) << std::endl;
                return false;
            }
            return true;
        }
        
        cl::Program& operator()() const {
            return *_program;
        }
        
        Context getContext() const {
            return _context;
        }
        
        cl::Kernel operator()(std::string kernelName) const {
            return cl::Kernel((*this)(), kernelName.c_str());
        }
    };
}

#endif /* E3B190A8_9B13_4CD5_8DE4_68B62FD3F10F */
