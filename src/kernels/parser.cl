#define CommaToken 1     // ",""
#define SemicolonToken 2 // ";"
#define LparenToken 3    // "("
#define RparenToken 4    // ")"
#define LbrackToken 5    //"["
#define RbrackToken 6    //"]"
#define LbraceToken 7    //"{"
#define RbraceToken 8    //"}"
#define DotToken 9       //"."
#define PlusToken 10     //"+"
#define MinusToken 11    //"-"
#define DivideToken 12   //"/"
#define TimesToken 13    //"*"
#define EqToken 14       //"="
#define AndToken 15      //"&"
#define OrToken 16       //"|"
#define CaretToken 17    //"^"
// special 2-length
#define GtToken 18     //">"
#define GeToken 19     //">="
#define LtToken 20     //"< "
#define NeqToken 21    //"<>"
#define LeToken 22     //"<="
#define ColonToken 23  // ": "
#define AssignToken 24 //":="
// cancer
#define ArrayToken 25    //"array"
#define BreakToken 37    //"break"
#define DoToken 32       //"do"
#define ElseToken 28     //"else"
#define EndToken 35      //"end"
#define ForToken 30      //"for"
#define FunctionToken 39 //"function"
#define InToken 34       //"in"
#define IfToken 26       //"if"
#define LetToken 33      //"let"
#define NilToken 38      //"nil"
#define OfToken 36       //"of"
#define ThenToken 27     //"then"
#define ToToken 31       //"to"
#define TypeToken 41     //"type"
#define VarToken 40      //"var"
#define WhileToken 29    //"while"
// Here goes all the special tokens...
#define IntToken 42
#define StringToken 43
#define IDToken 44

inline bool checkString(char *strPtr, char *tokPtr, const char *text) {}

__kernel void lexer(__global char *string, __global char *tokens,
                    __global char *data) {
  int strIdx = 0;
  int tokIdx = 0;
  while (string[strIdx] != '\0') {
    bool found = false;

    // while (string[strIdx] == ' ') {
    //   strIdx++;
    // }

    switch (string[strIdx]) {
    case ',':
      tokens[tokIdx] = CommaToken;
      break;
    case ';':
      tokens[tokIdx] = SemicolonToken;
      break;
    case '(':
      tokens[tokIdx] = LparenToken;
      break;
    case ')':
      tokens[tokIdx] = RparenToken;
      break;
    case '[':
      tokens[tokIdx] = LbrackToken;
      break;
    case ']':
      tokens[tokIdx] = RbrackToken;
      break;
    case '{':
      tokens[tokIdx] = LbraceToken;
      break;
    case '}':
      tokens[tokIdx] = RbraceToken;
      break;
    case '.':
      tokens[tokIdx] = DotToken;
      break;
    case '+':
      tokens[tokIdx] = PlusToken;
      break;
    case '-':
      tokens[tokIdx] = MinusToken;
      break;
    case '/':
      tokens[tokIdx] = DivideToken;
      break;
    case '*':
      tokens[tokIdx] = TimesToken;
      break;
    case '=':
      tokens[tokIdx] = EqToken;
      break;
    case '&':
      tokens[tokIdx] = AndToken;
      break;
    case '|':
      tokens[tokIdx] = OrToken;
      break;
    case '^':
      tokens[tokIdx] = CaretToken;
      break;
    // 2 length from here
    case '>':
      switch (string[strIdx + 1]) {
      case '=':
        tokens[tokIdx] = GeToken;
        strIdx++;
        break;
      default:
        tokens[tokIdx] = GtToken;
        break;
      }
      break;
    case '<':
      switch (string[strIdx + 1]) {
      case '=':
        tokens[tokIdx] = LeToken;
        strIdx++;
        break;
      case '>':
        tokens[tokIdx] = NeqToken;
        strIdx++;
        break;
      default:
        tokens[tokIdx] = LtToken;
        break;
      }
      break;
    case ':':
      switch (string[strIdx + 1]) {
      case '=':
        tokens[tokIdx] = AssignToken;
        strIdx++;
        break;
      default:
        tokens[tokIdx] = ColonToken;
        break;
      }
      break;
    case 'a':

    default:
      break;
    }
    strIdx++;
    tokIdx++;
  }
}
