#include "tokens.hpp"
#include <sstream>

token* IntTokenCheck::findCreate(std::ifstream& stream) {
    std::string tmp = "";
    char token;

    while(std::isdigit(stream.peek())) {
        stream.get(token);
        tmp += token;
    }

    if(tmp == "") {
        return nullptr;
    }

    return new IntToken(std::atoi(tmp.c_str()));
}

std::string IntToken::getToken() const {
    return getType().name() + (std::string)": " + ToString();
}

std::string IntToken::ToString() const {
    std::stringstream strStr;
    strStr << Value;
    return strStr.str();
}

token* StringTokenCheck::findCreate(std::ifstream& stream) {
    // TODO: Implement escaped chars!
    char chr;
    if (stream.peek() != '"') {
        return nullptr;
    }

    stream.get(chr);
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

std::string StringToken::getToken() const {
    return getType().name() + (std::string)": " + ToString();
}

std::string StringToken::ToString() const {
    std::stringstream strStr;
    strStr << '"' << Value << '"';
    return strStr.str();
}

token* IDTokenCheck::findCreate(std::ifstream& stream) {
    std::string tmp = "";
    char chr;

    if (!std::isalpha(stream.peek())) {
        return nullptr;
    }

    while(true) {
        stream.get(chr);

        if (!std::isalnum(chr) && chr != '_') {
            break;
        }

        tmp += chr;
    }

    stream.seekg(-1, std::ios_base::cur);
    return new IDToken(tmp);
}

std::string IDToken::getToken() const {
    return getType().name() + (std::string)": " + ToString();
}

std::string IDToken::ToString() const {
    return Name;
}