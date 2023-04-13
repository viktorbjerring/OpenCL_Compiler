#include"lexer.hpp"
#include<iostream>

std::list<tokenCheck*> Lexer::_tokens = {
    new CommaTokenCheck(),
    new SemicolonTokenCheck(),
    new AssignTokenCheck(),
    new ArrayTokenCheck,
    new IfTokenCheck(),
    new OrTokenCheck(),
    new AndTokenCheck(),
    new DoTokenCheck(),
    new FunctionTokenCheck(),
    new BreakTokenCheck(),
    new TypeTokenCheck(),
    new LetTokenCheck(),
    new InTokenCheck(),
    new LbraceTokenCheck(),
    new RbraceTokenCheck(),
    new LbrackTokenCheck(),
    new RbrackTokenCheck(),
    new WhileTokenCheck(),
    new ForTokenCheck(),
    new ToTokenCheck(),
    new OfTokenCheck(),
    new MinusTokenCheck(),
    new LparenTokenCheck(),
    new RparenTokenCheck(),
    new ThenTokenCheck(),
    new ElseTokenCheck(),
    new TimesTokenCheck(),
    new DivideTokenCheck(),
    new CaretTokenCheck(),
    new NeqTokenCheck(),
    new LeTokenCheck(),
    new GeTokenCheck(),
    new GtTokenCheck(),
    new LtTokenCheck(),
    new ColonTokenCheck(),
    new EqTokenCheck(),
    new PlusTokenCheck(),
    new VarTokenCheck(),
    new NilTokenCheck(),
    new EndTokenCheck(),
    new DotTokenCheck(),
    // Here goes all the special tokens...
    new IntTokenCheck(),
    new StringTokenCheck(),
    new IDTokenCheck()
};

Lexer::~Lexer() {
    for(auto token : _fileTokens) {
        delete token;
    }
}

void Lexer::ReadTokens(std::ifstream stream) {
    while(!stream.eof()) {
        bool found = false;
        
        for(auto Checker : _tokens) {
            auto tmp = Checker->findCreate(stream);
            
            if(tmp) {
                _fileTokens.push_back(tmp);
                found = true;
                break;
            }
        }
        
        if(!found) {
            throw std::runtime_error("What the fuck are this token???!?");
        }
    }
}