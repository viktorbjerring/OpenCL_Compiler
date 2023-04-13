#ifndef D4304121_4EB9_43F0_A054_15BCDA82D6C3
#define D4304121_4EB9_43F0_A054_15BCDA82D6C3

#include <fstream>
#include <string>
#include <list>
#include <cstring>
#include <typeinfo>
#include "tokens.hpp"
class Lexer {
  private:
    static std::list<tokenCheck*> _tokens;
    
    std::list<token*> _fileTokens;
    
  public:
    ~Lexer();
    
    void ReadTokens(std::ifstream stream);
    
}
#endif /* D4304121_4EB9_43F0_A054_15BCDA82D6C3 */
