#include"lexer.hpp"
#include<iostream>


token*  IntTokenCheck::findCreate(std::ifstream& stream) {
    for(auto chr : token) {
        if(chr > '9' || chr < '0') {
            return nullptr;
        }
    }

    return true;
}

token*  IfTokenCheck::findCreate(std::ifstream& stream) {
    if(token != "if") {
        return nullptr;
    }

    return true;
}