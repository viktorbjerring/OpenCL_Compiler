#include <iostream>

#include "../openCL/context.hpp"
#include "../openCL/program.hpp"

#ifndef CEBC928B_1EE5_4152_AAB7_A0E570249CA8
#define CEBC928B_1EE5_4152_AAB7_A0E570249CA8

class parser
{
private:
    open_cl::Context _context;
    open_cl::Program _program;

    cl::Kernel _parser;

    char *_tokens;
    char *_data;

public:
    parser() = default;
    parser(open_cl::Context &context)
    {
        setContext(context);
        _tokens = new char[20];
        _data = new char[20];
    }

    ~parser()
    {
        delete[] _tokens;
        delete[] _data;
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

    char *getTokens() const
    {
        return _tokens;
    }

    char *getData() const
    {
        return _data;
    }

    bool operator()()
    {
        cl::Buffer string_buffer(_context(), CL_MEM_READ_WRITE, 20 * sizeof(char));
        cl::Buffer token_buffer(_context(), CL_MEM_READ_WRITE, 20 * sizeof(char));
        cl::Buffer data_buffer(_context(), CL_MEM_WRITE_ONLY, 20 * sizeof(char));
        char *string[20];

        memset(string, 0, 20);
        memset(_tokens, 0, 20);
        const char testcode[] = "function () if in";

        memcpy(string, testcode, strlen(testcode) + 1);

        auto ret = _context.getContextQueue().enqueueWriteBuffer(string_buffer, CL_TRUE, 0, 20 * sizeof(char), string);

        if (ret)
        {
            std::cout << "write: " << ret << std::endl;
            return false;
        }

        ret = _context.getContextQueue().enqueueFillBuffer(token_buffer, "0", 0, 20 * sizeof(char));

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

        ret = _context.getContextQueue().enqueueNDRangeKernel(_parser, cl::NullRange, cl::NDRange(1));

        if (ret)
        {
            std::cout << "ndrange: " << ret << std::endl;
            return false;
        }

        ret = _context.getContextQueue().enqueueReadBuffer(token_buffer, CL_TRUE, 0, 20 * sizeof(char), _tokens);

        if (ret)
        {
            std::cout << "read: " << ret << std::endl;
            return false;
        }

        ret = _context.getContextQueue().enqueueReadBuffer(data_buffer, CL_TRUE, 0, 20 * sizeof(char), _data);

        if (ret)
        {
            std::cout << "read: " << ret << std::endl;
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
