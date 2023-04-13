#include "tokens.hpp"
#include <sstream>

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
    
    stream.seekg(-1, std::ios_base::cur);
    
    if(tmp == "") {
        return nullptr;
    }
    
    return new IntToken(std::atoi(tmp.c_str()));
}

std::string IntToken::ToString() const {
    std::stringstream strStr;
    strStr << Value;
    return strStr.str();
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
        stream.read(&chr, 1);
        
        if(chr == '"') {
            break;
        }
        
        tmp += chr;
    }
    
    stream.seekg(-1, std::ios_base::cur);
    return new StringToken(tmp);
}

std::string StringToken::ToString() const {
    std::stringstream strStr;
    strStr << '"' << Value << '"';
    return strStr.str();
}

token* IDTokenCheck::findCreate(std::ifstream& stream) {
    // TODO: Implement this!
    return nullptr;
}

std::string IDToken::ToString() const {
    return Name;
}