#include "parser.hpp"

void Parser::parseTokens(std::list<token*> _tokens) {
    Exp *expr = Exp::ConvertToken(_tokens);
}