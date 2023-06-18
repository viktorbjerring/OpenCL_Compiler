#include"program.hpp"
namespace open_cl {
    Program::Program(Context& context) {
        _context = context;
    }
    Program::~Program() {
        if(_program != nullptr) {
            delete _program;
        }
    }
    Program::Program(const Program& toCopy) {
        _context = toCopy._context;
        
        if(toCopy._program != nullptr) {
            _program = new cl::Program();
            *_program = *toCopy._program;
        }
    }
    Program::Program(Program&& toMove) {
        _context = toMove._context;
        
        if(toMove._program != nullptr) {
            _program = toMove._program;
            toMove._program = nullptr;
        }
    }
    const Program& Program::operator=(const Program& toCopy) {
        _context = toCopy._context;
        
        if(toCopy._program != nullptr) {
            _program = new cl::Program();
            *_program = *toCopy._program;
        }
        
        return *this;
    }
    const Program& Program::operator=(Program&& toMove) {
        _context = toMove._context;
        
        if(toMove._program != nullptr) {
            _program = toMove._program;
            toMove._program = nullptr;
        }
        
        return *this;
    }
    
    void Program::setContext(Context& context) {
        _context = context;
    }
    
    bool Program::open(std::string path) {
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
    
    bool Program::openBinary(std::string path) {
        std::ifstream ifs(path);
        
        if(!ifs.is_open()) {
            std::cerr << "Failed to load kernel.\n";
            return false;
        }
        
        std::string kernel_code((std::istreambuf_iterator<char>(ifs)),
                                (std::istreambuf_iterator<char>()));
        std::pair<const void*, size_t> binary;
        binary = {kernel_code.c_str(), kernel_code.length()};
        cl::Program::Binaries binaries;
        binaries.push_back(binary);
        _program = new cl::Program(_context(), {_context.getDevice()}, binaries);
        
        if(_program->build({_context.getDevice()}) != CL_SUCCESS) {
            std::cerr << "Error building: " << _program->getBuildInfo<CL_PROGRAM_BUILD_LOG>(_context.getDevice()) << std::endl;
            return false;
        }
        return true;
    }
    
    cl::Program& Program::operator()() const {
        return *_program;
    }
    
    Context Program::getContext() const {
        return _context;
    }
    
    cl::Kernel Program::operator()(std::string kernelName) const {
        return cl::Kernel((*this)(), kernelName.c_str());
    }
}