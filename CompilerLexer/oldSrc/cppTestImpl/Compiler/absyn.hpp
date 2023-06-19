#ifndef D77E7743_F9C5_47E3_A87E_C83E2ACC662D
#define D77E7743_F9C5_47E3_A87E_C83E2ACC662D

#include "tokens.hpp"

class Symbol {
private:
    Symbol(char *);
    static std::list<Symbol*> _symbols;
    char* name;
public:
    static Symbol* getOrCreate(char* name);
};

class Exp {
public:
    static Exp* ConvertToken(std::list<token*> _tokens);
    virtual Exp* checkAndCreate(std::list<token*> _tokens) = 0;
};

class Var {
public:
    static Var* checkAndCreate(std::list<token*> _tokens);
};


// EXPS

class VarExp : public Exp {
public:
    Exp* checkAndCreate(std::list<token*> _tokens) override;
private:
    Var* var;
};

class NilExp : public Exp {
public:
    Exp* checkAndCreate(std::list<token*> _tokens) override;
};

class IntExp : public Exp {
public:
    Exp* checkAndCreate(std::list<token*> _tokens) override;
private:
    int val;
};

class StringExp : public Exp {
public:
    Exp* checkAndCreate(std::list<token*> _tokens) override;
private:
    char* str;
};

class CallExp : public Exp {
public:
    Exp* checkAndCreate(std::list<token*> _tokens) override;
private:
    Symbol* id;
    Exp* args[];
};

class OpExp : public Exp {
public:
    Exp* checkAndCreate(std::list<token*> _tokens) override;
private:
    Exp* left;
    //Op op  ------- dunno if this should be enum or smth
    Exp* right;
};

class RecordExp : public Exp {
public:
    Exp* checkAndCreate(std::list<token*> _tokens) override;
public:
    struct Field {
        Symbol* id;
        Exp* exp;
    };
private:
    Symbol* id;
    Field fields[];
};

class SeqExp : public Exp {
public:
    SeqExp(std::list<token*>);
    Exp* checkAndCreate(std::list<token*> _tokens) override;
private:
    Exp* seq[];
};

class AssignExp : public Exp {
public:
    Exp* checkAndCreate(std::list<token*> _tokens) override;
private:
    Var* var;
    Exp* exp;
};

class IfExp : public Exp {
public:
    Exp* checkAndCreate(std::list<token*> _tokens) override;
private:
    Exp* test;
    Exp* then;
    Exp* els;
};

class WhileExp : public Exp {
public:
    Exp* checkAndCreate(std::list<token*> _tokens) override;
private:
    Exp* test;
    Exp* body;
};

class ForExp : public Exp {
public:
    Exp* checkAndCreate(std::list<token*> _tokens) override;
private:
    Symbol* symbol;
    // escape???
    Exp* low;
    Exp* high;
    Exp* body;
};

class BreakExp : public Exp {
public:
    Exp* checkAndCreate(std::list<token*> _tokens) override;
private:
    Var* var;
};

class LetExp : public Exp {
public:
    Exp* checkAndCreate(std::list<token*> _tokens) override;
private:
    //decls???
    Exp* body;
};

class ArrayExp : public Exp {
public:
    Exp* checkAndCreate(std::list<token*> _tokens) override;
private:
    Symbol* type;
    Exp* size;
    Exp* init;
};

// VARS

class SimpleVar : public Var {
    Symbol* symbol;
};

class FieldVar : public Var {
    Var* var;
    Symbol* symbol;
};

class SubscriptVar : public Var {
    Var* var;
    Exp* exp;
};

// EXP CHECKERS

class ExpChecker {
    virtual Exp* findCreate(std::list<token*> _tokens) = 0;
};

class VarExpChecker : public ExpChecker {
    Exp* findCreate(std::list<token*> _tokens);
};

class NilExpChecker : public ExpChecker {
    Exp* findCreate(std::list<token*> _tokens);
};

class IntExpChecker : public ExpChecker {
    Exp* findCreate(std::list<token*> _tokens);
};

class VarExpChecker : public ExpChecker {
    Exp* findCreate(std::list<token*> _tokens);
};

class VarExpChecker : public ExpChecker {
    Exp* findCreate(std::list<token*> _tokens);
};

class VarExpChecker : public ExpChecker {
    Exp* findCreate(std::list<token*> _tokens);
};

class VarExpChecker : public ExpChecker {
    Exp* findCreate(std::list<token*> _tokens);
};

class VarExpChecker : public ExpChecker {
    Exp* findCreate(std::list<token*> _tokens);
};

class VarExpChecker : public ExpChecker {
    Exp* findCreate(std::list<token*> _tokens);
};


#endif /* D77E7743_F9C5_47E3_A87E_C83E2ACC662D */
