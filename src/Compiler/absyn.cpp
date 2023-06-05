#include "absyn.hpp"

#define CheckToken(type, ptr) \
    if (!type##Token::isType(*ptr)) { \
        return nullptr; \
    }

#define EatToken(type, ptr) \
    CheckToken(type, ptr) \
    _tokens.erase(ptr);

Symbol* Symbol::getOrCreate(char* name) {
    for (auto symb : _symbols) {
        if (name == (*symb).name) {
            return symb;
        }
        Symbol *created = new Symbol(name);
        _symbols.emplace_back(created);
        return created;
    }
}

Exp* Exp::ConvertToken(std::list<token*> _tokens) {

}

Exp* VarExp::checkAndCreate(std::list<token*> _tokens) {

}

Exp* NilExp::checkAndCreate(std::list<token*> _tokens) {
    auto tokPtr = _tokens.begin();
    CheckToken(Nil, tokPtr);
    _tokens.erase(tokPtr);
    return new NilExp();
}

Exp* IntExp::checkAndCreate(std::list<token*> _tokens) {

}

Exp* StringExp::checkAndCreate(std::list<token*> _tokens) {

}

Exp* CallExp::checkAndCreate(std::list<token*> _tokens) {

}

Exp* OpExp::checkAndCreate(std::list<token*> _tokens) {

}

Exp* RecordExp::checkAndCreate(std::list<token*> _tokens) {

}

Exp* SeqExp::checkAndCreate(std::list<token*> _tokens) {
    auto tokPtr = _tokens.begin();
    CheckToken(Lparen, tokPtr)
    auto seqBody = std::list<token*>();
    while(!RparenToken::isType(*++tokPtr)) {
        seqBody.push_back(*tokPtr);
    }

}

Exp* AssignExp::checkAndCreate(std::list<token*> _tokens) {

}

Exp* ForExp::checkAndCreate(std::list<token*> _tokens) {
    auto tokPtr = _tokens.begin();
}

Exp* IfExp::checkAndCreate(std::list<token*> _tokens) {
    EatToken(If, _tokens.begin())
    auto startPtr = _tokens.begin();

}

Exp* WhileExp::checkAndCreate(std::list<token*> _tokens) {

}

Exp* BreakExp::checkAndCreate(std::list<token*> _tokens) {

}

Exp* LetExp::checkAndCreate(std::list<token*> _tokens) {

}

Exp* ArrayExp::checkAndCreate(std::list<token*> _tokens) {

}