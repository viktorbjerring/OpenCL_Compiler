#ifndef BCA2BBD7_95B8_4B3D_9E68_EB92AF5BB0A3
#define BCA2BBD7_95B8_4B3D_9E68_EB92AF5BB0A3

#include "absyn.hpp"

class Parser {
public:
    void parseTokens(std::list<token*> _tokens);
private:
    std::list<Exp*> _exps;
};

#endif /* BCA2BBD7_95B8_4B3D_9E68_EB92AF5BB0A3 */
