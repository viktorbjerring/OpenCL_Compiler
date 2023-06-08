#include <iostream>

#include "../openCL/context.hpp"
#include "../openCL/program.hpp"

#ifndef CEBC928B_1EE5_4152_AAB7_A0E570249CA8
#define CEBC928B_1EE5_4152_AAB7_A0E570249CA8

#define CHAR_PER_TOKEN_DATA 2

#define STRING_LENGTH 5000

#define TOKEN_BUFFER_SIZE STRING_LENGTH / CHAR_PER_TOKEN_DATA
#define DATA_BUFFER_SIZE STRING_LENGTH / CHAR_PER_TOKEN_DATA

class parser
{
private:
    open_cl::Context _context;
    open_cl::Program _program;

    cl::Kernel _parser;

    char *_input;
    int _input_len;
    char *_tokens;
    char *_data;
    int *_retVal;

public:
    parser() = default;
    parser(open_cl::Context &context)
    {
        setContext(context);
        _tokens = new char[TOKEN_BUFFER_SIZE];
        _data = new char[DATA_BUFFER_SIZE];
        _retVal = new int(TOKEN_BUFFER_SIZE);

        // memset(_tokens, 0, TOKEN_BUFFER_SIZE);
        // memset(_data, 0, DATA_BUFFER_SIZE);
    }

    ~parser()
    {
        delete[] _tokens;
        delete[] _data;
        delete[] _retVal;
    }

    inline void setContext(open_cl::Context &context)
    {
        _context = context;
        _program = open_cl::Program(_context);
        auto ret = _program.open("parser.cl");
        if (!ret)
        {
            std::cout << "error in open" << std::endl;
        }
        _parser = _program("lexer");
    }

    void setInput(char *input, const size_t len)
    {
        _input = input;
        _input_len = len;
    }

    char *getTokens() const
    {
        return _tokens;
    }

    char *getData() const
    {
        return _data;
    }

    int *getRetVal() const
    {
        return _retVal;
    }

    bool operator()()
    {
        cl::Buffer string_buffer(_context(), CL_MEM_READ_WRITE, STRING_LENGTH * sizeof(char));
        cl::Buffer token_buffer(_context(), CL_MEM_READ_WRITE, TOKEN_BUFFER_SIZE * sizeof(char));
        cl::Buffer data_buffer(_context(), CL_MEM_WRITE_ONLY, DATA_BUFFER_SIZE * sizeof(char));
        cl::Buffer retVal_buffer(_context(), CL_MEM_WRITE_ONLY, sizeof(int));

        auto ret = _context.getContextQueue().enqueueWriteBuffer(string_buffer, CL_TRUE, 0, STRING_LENGTH * sizeof(char), _input);
        ret = _context.getContextQueue().enqueueWriteBuffer(retVal_buffer, CL_TRUE, 0, sizeof(int), _retVal);

        if (ret)
        {
            std::cout << "write: " << ret << std::endl;
            return false;
        }

        ret = _context.getContextQueue().enqueueFillBuffer(token_buffer, 0, 0, TOKEN_BUFFER_SIZE * sizeof(char));
        ret = _context.getContextQueue().enqueueFillBuffer(data_buffer, 0, 0, DATA_BUFFER_SIZE * sizeof(char));

        ret = _parser.setArg(0, string_buffer);
        if (ret)
        {
            std::cout << "setarg0: " << ret << std::endl;
            return false;
        }
        ret = _parser.setArg(1, token_buffer);
        if (ret)
        {
            std::cout << "setarg1: " << ret << std::endl;
            return false;
        }
        ret = _parser.setArg(2, data_buffer);
        if (ret)
        {
            std::cout << "setarg2: " << ret << std::endl;
            return false;
        }

        ret = _parser.setArg(3, retVal_buffer);
        if (ret)
        {
            std::cout << "setarg3: " << ret << std::endl;
            return false;
        }

        ret = _context.getContextQueue().enqueueNDRangeKernel(_parser, cl::NullRange, cl::NDRange(1));

        if (ret)
        {
            std::cout << "ndrange: " << ret << std::endl;
            return false;
        }

        ret = _context.getContextQueue().enqueueReadBuffer(token_buffer, CL_TRUE, 0, TOKEN_BUFFER_SIZE * sizeof(char), _tokens);

        if (ret)
        {
            std::cout << "read_tok: " << ret << std::endl;
            return false;
        }

        ret = _context.getContextQueue().enqueueReadBuffer(data_buffer, CL_TRUE, 0, DATA_BUFFER_SIZE * sizeof(char), _data);

        if (ret)
        {
            std::cout << "read_dat: " << ret << std::endl;
            return false;
        }

        ret = _context.getContextQueue().enqueueReadBuffer(retVal_buffer, CL_TRUE, 0, sizeof(int), _retVal);

        if (ret)
        {
            std::cout << "read_cnt: " << ret << std::endl;
            return false;
        }

        ret = _context.getContextQueue().finish();

        if (ret)
        {
            std::cout << "finish: " << ret << std::endl;
            return false;
        }

        return true;
    }
};

#endif /* CEBC928B_1EE5_4152_AAB7_A0E570249CA8 */
