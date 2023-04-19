#include <fstream>
#include <string>
#include <list>
#include <cstring>
#include <typeinfo>
#ifndef AB9934A9_6EA2_4A1C_A80E_78A98C4680BC
#define AB9934A9_6EA2_4A1C_A80E_78A98C4680BC

// Token helping macros
#define TOKEN_CLASS(class_name)                                                             \
    public:                                                                                 \
    static inline bool isType(token* toCheck) {                                             \
        return toCheck->getType() == typeid(class_name);                                    \
    }                                                                                       \
    virtual const std::type_info& getType() const override {                                \
        return typeid(class_name);                                                          \
    }                                                                                       \
    private:

#define TOKEN_CHECK(token_name)                                                             \
    class token_name##Check : public tokenCheck {                                           \
      public:                                                                               \
        virtual token* findCreate(std::ifstream& stream) override;                          \
    };

#define SIMPLE_TOKEN_CLASS(token_name, token_symbol)                                        \
    class token_name##Token : public token {                                                \
        TOKEN_CLASS(token_name##Token)                                                      \
      public:                                                                               \
        virtual std::string ToString() const override {                                     \
            return token_symbol;                                                            \
        }                                                                                   \
    };                                                                                      \
    class token_name##TokenCheck : public tokenCheck {                                      \
      public:                                                                               \
        virtual token* findCreate(std::ifstream& stream) override {                         \
            char token[sizeof(token_symbol)];                                               \
            stream.read(token, sizeof(token_symbol));                                       \
            if(std::strcmp(token, token_symbol)) {                                          \
                stream.seekg(-static_cast<int>(sizeof(token_symbol)), std::ios_base::cur);  \
                return nullptr;                                                             \
            }                                                                               \
            return new token_name##Token();                                                 \
        }                                                                                   \
    };

// Base classes
class token {
  public:
    virtual const std::type_info& getType() const = 0;
    virtual std::string ToString() const = 0;
};

class tokenCheck {
  public:
    virtual token* findCreate(std::ifstream& stream) = 0;
};

// Complex tokens
class IntToken : public token {
    TOKEN_CLASS(IntToken)
    int Value;
  public:
    IntToken(int value) : Value(value) {}
    virtual std::string ToString() const override;
};
TOKEN_CHECK(IntToken)

class IDToken : public token {
    TOKEN_CLASS(IDToken)
    std::string Name;
  public:
    IDToken(std::string name) : Name(name) {}
    virtual std::string ToString() const override;
};
TOKEN_CHECK(IDToken)

class StringToken : public token {
    TOKEN_CLASS(StringToken)
    std::string Value;
  public:
    StringToken(std::string value) : Value(value) {}
    virtual std::string ToString() const override;
};
TOKEN_CHECK(StringToken)

// Simple tokens
SIMPLE_TOKEN_CLASS(Comma, ",")
SIMPLE_TOKEN_CLASS(Colon, ":")
SIMPLE_TOKEN_CLASS(Semicolon, ";")
SIMPLE_TOKEN_CLASS(Lparen, "(")
SIMPLE_TOKEN_CLASS(Rparen, ")")
SIMPLE_TOKEN_CLASS(Lbrack, "[")
SIMPLE_TOKEN_CLASS(Rbrack, "]")
SIMPLE_TOKEN_CLASS(Lbrace, "{")
SIMPLE_TOKEN_CLASS(Rbrace, "}")
SIMPLE_TOKEN_CLASS(Dot, ".")
SIMPLE_TOKEN_CLASS(Plus, "+")
SIMPLE_TOKEN_CLASS(Minus, "-")
SIMPLE_TOKEN_CLASS(Divide, "/")
SIMPLE_TOKEN_CLASS(Times, "*")
SIMPLE_TOKEN_CLASS(Eq, "=")
SIMPLE_TOKEN_CLASS(Neq, "<>")
SIMPLE_TOKEN_CLASS(Lt, "<")
SIMPLE_TOKEN_CLASS(Le, "<=")
SIMPLE_TOKEN_CLASS(Gt, ">")
SIMPLE_TOKEN_CLASS(Ge, ">=")
SIMPLE_TOKEN_CLASS(And, "&")
SIMPLE_TOKEN_CLASS(Or, "|")
SIMPLE_TOKEN_CLASS(Caret, "^")
SIMPLE_TOKEN_CLASS(Assign, ":=")
SIMPLE_TOKEN_CLASS(Array, "array")
SIMPLE_TOKEN_CLASS(If, "if")
SIMPLE_TOKEN_CLASS(Then, "then")
SIMPLE_TOKEN_CLASS(Else, "else")
SIMPLE_TOKEN_CLASS(While, "while")
SIMPLE_TOKEN_CLASS(For, "for")
SIMPLE_TOKEN_CLASS(To, "to")
SIMPLE_TOKEN_CLASS(Do, "do")
SIMPLE_TOKEN_CLASS(Let, "let")
SIMPLE_TOKEN_CLASS(In, "in")
SIMPLE_TOKEN_CLASS(End, "end")
SIMPLE_TOKEN_CLASS(Of, "of")
SIMPLE_TOKEN_CLASS(Break, "break")
SIMPLE_TOKEN_CLASS(Nil, "nil")
SIMPLE_TOKEN_CLASS(Function, "function")
SIMPLE_TOKEN_CLASS(Var, "var")
SIMPLE_TOKEN_CLASS(Type, "type")

#endif /* AB9934A9_6EA2_4A1C_A80E_78A98C4680BC */
