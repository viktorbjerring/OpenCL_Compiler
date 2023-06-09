#include <iostream>

#include "openCL/context.hpp"
#include "openCL/program.hpp"
#include "helpers/printTime.hpp"

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
    cl::Event* _writeEvent;
    cl::Event* _runEvent;
    cl::Event* _readEvent;
    
    cl::Kernel _lexer;
    
    char* _input;
    int _input_len;
    char* _tokens;
    char* _data;
    int* _retVal;
    
  public:
    lexer() = default;
    lexer(open_cl::Context& context) {
        setContext(context);
        _tokens = new char[TOKEN_BUFFER_SIZE];
        _data = new char[DATA_BUFFER_SIZE];
        _retVal = new int;
        *_retVal = TOKEN_BUFFER_SIZE;
        _writeEvent = new cl::Event();
        _runEvent = new cl::Event();
        _readEvent = new cl::Event();
        // memset(_tokens, 0, TOKEN_BUFFER_SIZE);
        // memset(_data, 0, DATA_BUFFER_SIZE);
    }
    
    ~lexer() {
        delete[] _tokens;
        delete[] _data;
        delete[] _retVal;
        delete _writeEvent;
        delete _runEvent;
        delete _readEvent;
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
    
    unsigned long getWrite() const {
        cl_ulong start, end;
        _writeEvent->getProfilingInfo(CL_PROFILING_COMMAND_END, &end);
        _writeEvent->getProfilingInfo(CL_PROFILING_COMMAND_START, &start);
        return end - start;
    }
    
    unsigned long getExecute() const {
        cl_ulong start, end;
        _runEvent->getProfilingInfo(CL_PROFILING_COMMAND_END, &end);
        _runEvent->getProfilingInfo(CL_PROFILING_COMMAND_START, &start);
        return end - start;
    }
    
    unsigned long getRead() const {
        cl_ulong start, end;
        _readEvent->getProfilingInfo(CL_PROFILING_COMMAND_END, &end);
        _readEvent->getProfilingInfo(CL_PROFILING_COMMAND_START, &start);
        return end - start;
    }
    
    bool operator()() {
        cl::Buffer string_buffer(_context(), CL_MEM_READ_WRITE, STRING_LENGTH * sizeof(char));
        cl::Buffer token_buffer(_context(), CL_MEM_READ_WRITE, TOKEN_BUFFER_SIZE * sizeof(char));
        cl::Buffer data_buffer(_context(), CL_MEM_WRITE_ONLY, DATA_BUFFER_SIZE * sizeof(char));
        cl::Buffer retVal_buffer(_context(), CL_MEM_READ_WRITE, sizeof(int));
        auto ret = _context.getContextQueue().enqueueWriteBuffer(string_buffer, CL_FALSE, 0, STRING_LENGTH * sizeof(char), _input, NULL, _writeEvent);
        cl::Event writeEvt;
        ret = _context.getContextQueue().enqueueWriteBuffer(retVal_buffer, CL_FALSE, 0, sizeof(int), _retVal, 0, &writeEvt);
        writeEvt.wait();
        
        if(ret) {
            std::cout << "write: " << ret << std::endl;
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
        
        std::vector<cl::Event> _waitWrite = {*_writeEvent};
        ret = _context.getContextQueue().enqueueNDRangeKernel(_lexer, cl::NullRange, cl::NDRange(1), cl::NullRange, &_waitWrite, _runEvent);
        
        if(ret) {
            std::cout << "ndrange: " << ret << std::endl;
            return false;
        }
        
        std::vector<cl::Event> _waitRead = {*_runEvent};
        ret = _context.getContextQueue().enqueueReadBuffer(token_buffer, CL_FALSE, 0, TOKEN_BUFFER_SIZE * sizeof(char), _tokens, &_waitRead, _readEvent);
        
        if(ret) {
            std::cout << "read_tok: " << ret << std::endl;
            return false;
        }
        
        ret = _context.getContextQueue().enqueueReadBuffer(data_buffer, CL_FALSE, 0, DATA_BUFFER_SIZE * sizeof(char), _data, &_waitRead);
        
        if(ret) {
            std::cout << "read_dat: " << ret << std::endl;
            return false;
        }
        
        ret = _context.getContextQueue().enqueueReadBuffer(retVal_buffer, CL_FALSE, 0, sizeof(int), _retVal, &_waitRead);
        
        if(ret) {
            std::cout << "read_cnt: " << ret << std::endl;
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
