#ifndef D4304121_4EB9_43F0_A054_15BCDA82D6C3
#define D4304121_4EB9_43F0_A054_15BCDA82D6C3

#include <fstream>
#include <string>
#include <list>
#include <cstring>
#include <typeinfo>

#define TOKEN_CLASS(class_name)                            \
    public:                                                 \
    static inline bool isType(token* toCheck) {             \
        return toCheck->getType() == typeid(class_name);    \
    }                                                       \
    virtual const std::type_info& getType() const override {\
        return typeid(class_name);                          \
    }                                                       \
    private:

#define TOKEN_CHECK(token_name)                  \
    class token_name##Check : public tokenCheck {           \
    public:                                                 \
    virtual token* findCreate(std::ifstream& stream) override; \
};

#define SIMPLE_TOKEN_CLASS(token_name, token_symbol)  \
    class token_name##Token : public token {   \
        TOKEN_CLASS(token_name##Token)         \
    };                                  \
    class token_name##TokenCheck : public tokenCheck { \
    public: \
    virtual token* findCreate(std::ifstream& stream) override { \
        char token[sizeof(token_symbol)];                        \
        stream.read(token, sizeof(token_symbol));                \
        if(std::strcmp(token, token_symbol)) {                   \
            stream.seekg(-sizeof(token_symbol), std::ios_base::cur);               \
            return nullptr;                                     \
        }                                                       \
        return new token_name##Token();                         \
    }                   \
    };

/// BASE classez
class token {
public:
    virtual const std::type_info& getType() const = 0;
};

class tokenCheck {
public:
    virtual token* findCreate(std::ifstream& stream) = 0;
};

class IntToken : public token {
    TOKEN_CLASS(IntToken)
    int Value;
public:
    IntToken(int value) : Value(value) {}
};
TOKEN_CHECK(IntToken)

class IDToken : public token {
    TOKEN_CLASS(IDToken)
    std::string Name;
public:
    IDToken(std::string name) : Name(name) {}
};
TOKEN_CHECK(IDToken)

class StringToken : public token {
    TOKEN_CLASS(StringToken)
    std::string Name;
public:
    StringToken(std::string name) : Name(name) {}
};
TOKEN_CHECK(StringToken, token.c_str())

SIMPLE_TOKEN_CLASS(Comma, ",")
SIMPLE_TOKEN_CLASS(Colon, ":")
SIMPLE_TOKEN_CLASS(Semicolon, ";")
SIMPLE_TOKEN_CLASS(Lparen)
SIMPLE_TOKEN_CLASS(Rparen)
SIMPLE_TOKEN_CLASS(Lbrack)
SIMPLE_TOKEN_CLASS(Rbrack)
SIMPLE_TOKEN_CLASS(Lbrace)
SIMPLE_TOKEN_CLASS(Rbrace)
SIMPLE_TOKEN_CLASS(Dot)
SIMPLE_TOKEN_CLASS(Plus)
SIMPLE_TOKEN_CLASS(Minus)
SIMPLE_TOKEN_CLASS(Divide)
SIMPLE_TOKEN_CLASS(Times)
SIMPLE_TOKEN_CLASS(Eq)
SIMPLE_TOKEN_CLASS(Neq)
SIMPLE_TOKEN_CLASS(Lt)
SIMPLE_TOKEN_CLASS(Le)
SIMPLE_TOKEN_CLASS(Gt)
SIMPLE_TOKEN_CLASS(Ge)
SIMPLE_TOKEN_CLASS(And)
SIMPLE_TOKEN_CLASS(Or)
SIMPLE_TOKEN_CLASS(Assign)
SIMPLE_TOKEN_CLASS(Array)
SIMPLE_TOKEN_CLASS(If)
SIMPLE_TOKEN_CLASS(Then)
SIMPLE_TOKEN_CLASS(Else)
SIMPLE_TOKEN_CLASS(While)
SIMPLE_TOKEN_CLASS(For)
SIMPLE_TOKEN_CLASS(To)
SIMPLE_TOKEN_CLASS(Do)
SIMPLE_TOKEN_CLASS(Let)
SIMPLE_TOKEN_CLASS(In)
SIMPLE_TOKEN_CLASS(End)
SIMPLE_TOKEN_CLASS(Of)
SIMPLE_TOKEN_CLASS(Break)
SIMPLE_TOKEN_CLASS(Nil)
SIMPLE_TOKEN_CLASS(Function)
SIMPLE_TOKEN_CLASS(Var)
SIMPLE_TOKEN_CLASS(Type)

/***************************
 * Splitline
****************************/



class Lexer {
private:
    static inline std::list<tokenCheck*> _tokens = {
        new IntTokenCheck(),
        new IfTokenCheck()
    };

    std::list<token*> fileTokens;

public:
    ~Lexer() {
        for(auto token : fileTokens) {
            delete token;
        }
    }

    void ReadTokens(std::list<std::string> Tokens) {
        for(auto Token : Tokens) {
            bool found = false;

            for(auto Checker : _tokens) {
                if(Checker->find(Token)) {
                    fileTokens.push_back(Checker->create(Token));
                    found = true;
                    break;
                }
            }

            if(!found) {
                throw std::runtime_error("What the fuck are this token???!?");
            }
        }
    }

};
#endif /* D4304121_4EB9_43F0_A054_15BCDA82D6C3 */
