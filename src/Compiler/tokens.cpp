#include "tokens.hpp"

token* IntTokenCheck::findCreate(std::ifstream& stream) {
    std::string tmp = "";
    char token;
    
    while(true) {
        stream.read(&token, 1);
        
        if(token > '9' || token < '0') {
            break;
        }
        
        tmp += token;
    }
    
    if(tmp == "") {
        return nullptr;
    }
    
    return new IntToken(std::atoi(tmp.c_str()));
}

token* StringTokenCheck::findCreate(std::ifstream& stream) {
    // TODO: Implement escaped chars!
    char chr;
    stream.read(&chr, 1);
    
    if(chr != '"') {
        return nullptr;
    }
    
    std::string tmp = "";
    
    while(true) {
        if(chr == '"') {
            break;
        }
        
        tmp += chr;
    }
    
    return new StringToken(tmp);
}