#ifndef DF8FD384_9897_4A31_A673_D1150B9AC9DB
#define DF8FD384_9897_4A31_A673_D1150B9AC9DB
#include <iostream>

#include "openCL/context.hpp"
#include "openCL/program.hpp"

#ifndef CEBC928B_1EE5_4152_AAB7_A0E570249CA8
#define CEBC928B_1EE5_4152_AAB7_A0E570249CA8

#define CHAR_PER_TOKEN_DATA 2

#define STRING_LENGTH 262144

#define TOKEN_BUFFER_SIZE (STRING_LENGTH / CHAR_PER_TOKEN_DATA)
#define DATA_BUFFER_SIZE (STRING_LENGTH / CHAR_PER_TOKEN_DATA)

class lexer {
  private:
    open_cl::Context _context;
    open_cl::Program _program;
    
    cl::Kernel _lexer;
    
    char* _input;
    int _input_len;
    char* _tokens;
    char* _data;
    int* _retVal;
    cl::Event _writeStringBufEvent;
    cl::Event _writeRetValEvent;
    cl::Event _runEvent;
    cl::Event _readTokenBufEvent;
    cl::Event _readDataBufEvent;
    cl::Event _readRetValEvent;
    
  public:
    lexer() = default;
    lexer(open_cl::Context& context) {
        setContext(context);
        // _tokens = new char[TOKEN_BUFFER_SIZE];
        // _data = new char[DATA_BUFFER_SIZE];
        // memset(_tokens, 0, TOKEN_BUFFER_SIZE);
        // memset(_data, 0, DATA_BUFFER_SIZE);
    }
    
    ~lexer() {
        // delete[] _tokens;
        // delete[] _data;
    }
    
    inline const cl::Event& getWriteStringBufEvent() const {
        return _writeStringBufEvent;
    }
    inline const cl::Event& getWriteRetValEvent() const {
        return _writeRetValEvent;
    }
    inline const cl::Event& getRunEvent() const {
        return _runEvent;
    }
    inline const cl::Event& getReadTokenBufEvent() const {
        return _readTokenBufEvent;
    }
    inline const cl::Event& getReadDataBufEvent() const {
        return _readDataBufEvent;
    }
    inline const cl::Event& getReadRetValEvent() const {
        return _readRetValEvent;
    }
    
    inline void setContext(open_cl::Context& context) {
        _context = context;
        _program = open_cl::Program(_context);
        auto ret = _program.open("lexer.cl");
        
        if(!ret) {
            std::cout << "error in open" << std::endl;
        }
        
        _lexer = _program("lexer");
    }
    
    void setInput(char* input, const size_t len) {
        _input = input;
        _input_len = len;
    }
    
    char* getTokens() const {
        return _tokens;
    }
    
    char* getData() const {
        return _data;
    }
    
    int* getRetVal() const {
        return _retVal;
    }
    
    bool operator()() {
        cl::Buffer string_buffer(_context(), CL_MEM_READ_WRITE, STRING_LENGTH * sizeof(char));
        cl::Buffer token_buffer(_context(), CL_MEM_READ_WRITE, TOKEN_BUFFER_SIZE * sizeof(char));
        cl::Buffer data_buffer(_context(), CL_MEM_WRITE_ONLY, DATA_BUFFER_SIZE * sizeof(char));
        cl::Buffer retVal_buffer(_context(), CL_MEM_READ_WRITE, sizeof(int));
        cl_int ret;
        ret = _context.getContextQueue().enqueueWriteBuffer(string_buffer, CL_FALSE, 0, STRING_LENGTH * sizeof(char), _input, NULL, &_writeStringBufEvent);
        _writeStringBufEvent.wait();
        
        if(ret) {
            std::cout << "write: " << ret << std::endl;
            return false;
        }
        
        _retVal = reinterpret_cast<int*>(_context.getContextQueue().enqueueMapBuffer(retVal_buffer, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE, 0, sizeof(int), 0, &_writeRetValEvent, &ret));
        _writeRetValEvent.wait();
        *_retVal = TOKEN_BUFFER_SIZE;
        
        if(ret) {
            std::cout << "write_val: " << ret << std::endl;
            return false;
        }
        
        _tokens =  reinterpret_cast<char*>(_context.getContextQueue().enqueueMapBuffer(token_buffer, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE, 0, TOKEN_BUFFER_SIZE * sizeof(char), nullptr, &_readTokenBufEvent, &ret));
        _readTokenBufEvent.wait();
        
        if(ret) {
            std::cout << "read_tok: " << ret << std::endl;
            return false;
        }
        
        _data = reinterpret_cast<char*>(_context.getContextQueue().enqueueMapBuffer(data_buffer, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE, 0, DATA_BUFFER_SIZE * sizeof(char), nullptr, &_readDataBufEvent, &ret));
        _readDataBufEvent.wait();
        
        if(ret) {
            std::cout << "read_dat: " << ret << std::endl;
            return false;
        }
        
        // ret = _context.getContextQueue().enqueueFillBuffer(token_buffer, 0, 0, TOKEN_BUFFER_SIZE * sizeof(char));
        // ret = _context.getContextQueue().enqueueFillBuffer(data_buffer, 0, 0, DATA_BUFFER_SIZE * sizeof(char));
        ret = _lexer.setArg(0, string_buffer);
        
        if(ret) {
            std::cout << "setarg0: " << ret << std::endl;
            return false;
        }
        
        ret = _lexer.setArg(1, token_buffer);
        
        if(ret) {
            std::cout << "setarg1: " << ret << std::endl;
            return false;
        }
        
        ret = _lexer.setArg(2, data_buffer);
        
        if(ret) {
            std::cout << "setarg2: " << ret << std::endl;
            return false;
        }
        
        ret = _lexer.setArg(3, retVal_buffer);
        
        if(ret) {
            std::cout << "setarg3: " << ret << std::endl;
            return false;
        }
        
        ret = _context.getContextQueue().enqueueNDRangeKernel(_lexer, cl::NullRange, cl::NDRange(1), cl::NullRange, nullptr, &_runEvent);
        _runEvent.wait();
        
        if(ret) {
            std::cout << "ndrange: " << ret << std::endl;
            return false;
        }
        
        ret = _context.getContextQueue().finish();
        
        if(ret) {
            std::cout << "finish: " << ret << std::endl;
            return false;
        }
        
        return true;
    }
};

#endif /* CEBC928B_1EE5_4152_AAB7_A0E570249CA8 */


#endif /* DF8FD384_9897_4A31_A673_D1150B9AC9DB */
