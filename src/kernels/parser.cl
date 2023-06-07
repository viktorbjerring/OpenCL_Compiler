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
#define BreakToken 26    //"break"
#define DoToken 27       //"do"
#define ElseToken 28     //"else"
#define EndToken 29      //"end"
#define ForToken 30      //"for"
#define FunctionToken 31 //"function"
#define InToken 32       //"in"
#define IfToken 33       //"if"
#define LetToken 34      //"let"
#define NilToken 35      //"nil"
#define OfToken 36       //"of"
#define ThenToken 37     //"then"
#define ToToken 38       //"to"
#define TypeToken 39     //"type"
#define VarToken 40      //"var"
#define WhileToken 41    //"while"
// Here goes all the special tokens...
#define IntToken 42
#define StringToken 43
#define IDToken 44

inline uchar checkString(volatile __global char *strPtr,
                         __constant const char *text) {
  int substrIdx = 0;
  while (text[substrIdx] != '\0') {
    if (strPtr[substrIdx] != text[substrIdx]) {
      return 0;
    }
    substrIdx++;
  }
  return substrIdx;
}

__kernel void lexer(__global char *string, __global char *tokens,
                    __global char *data) {
  int strIdx = 0;
  int dataIdx = 0;
  int tokIdx = 0;
  int tempIdx = 0;
  while (string[strIdx] != '\0') {
    bool found = false;

    while (string[strIdx] == ' ') {
      strIdx++;
    }

    switch (string[strIdx]) {
    case ',':
      data[dataIdx++] = strIdx;
      tokens[tokIdx++] = CommaToken;
      strIdx++;
      break;
    case ';':
      data[dataIdx++] = strIdx;
      tokens[tokIdx++] = SemicolonToken;
      strIdx++;
      break;
    case '(':
      data[dataIdx++] = strIdx;
      tokens[tokIdx++] = LparenToken;
      strIdx++;
      break;
    case ')':
      data[dataIdx++] = strIdx;
      tokens[tokIdx++] = RparenToken;
      strIdx++;
      break;
    case '[':
      data[dataIdx++] = strIdx;
      tokens[tokIdx++] = LbrackToken;
      strIdx++;
      break;
    case ']':
      data[dataIdx++] = strIdx;
      tokens[tokIdx++] = RbrackToken;
      strIdx++;
      break;
    case '{':
      data[dataIdx++] = strIdx;
      tokens[tokIdx++] = LbraceToken;
      strIdx++;
      break;
    case '}':
      data[dataIdx++] = strIdx;
      tokens[tokIdx++] = RbraceToken;
      strIdx++;
      break;
    case '.':
      data[dataIdx++] = strIdx;
      tokens[tokIdx++] = DotToken;
      strIdx++;
      break;
    case '+':
      data[dataIdx++] = strIdx;
      tokens[tokIdx++] = PlusToken;
      strIdx++;
      break;
    case '-':
      data[dataIdx++] = strIdx;
      tokens[tokIdx++] = MinusToken;
      strIdx++;
      break;
    case '/':
      data[dataIdx++] = strIdx;
      tokens[tokIdx++] = DivideToken;
      strIdx++;
      break;
    case '*':
      data[dataIdx++] = strIdx;
      tokens[tokIdx++] = TimesToken;
      strIdx++;
      break;
    case '=':
      data[dataIdx++] = strIdx;
      tokens[tokIdx++] = EqToken;
      strIdx++;
      break;
    case '&':
      data[dataIdx++] = strIdx;
      tokens[tokIdx++] = AndToken;
      strIdx++;
      break;
    case '|':
      data[dataIdx++] = strIdx;
      tokens[tokIdx++] = OrToken;
      strIdx++;
      break;
    case '^':
      data[dataIdx++] = strIdx;
      tokens[tokIdx++] = CaretToken;
      strIdx++;
      break;
    // 2 length from here
    case '>':
      switch (string[strIdx + 1]) {
      case '=':
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = GeToken;
        strIdx += 2;
        break;
      default:
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = GtToken;
        strIdx++;
        break;
      }
      break;
    case '<':
      switch (string[strIdx + 1]) {
      case '=':
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = LeToken;
        strIdx += 2;
        break;
      case '>':
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = NeqToken;
        strIdx += 2;
        break;
      default:
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = LtToken;
        strIdx++;
        break;
      }
      break;
    case ':':
      switch (string[strIdx + 1]) {
      case '=':
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = AssignToken;
        strIdx += 2;
        break;
      default:
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = ColonToken;
        strIdx++;
        break;
      }
      break;
    case 'a':
      tempIdx = checkString((string + strIdx), "array");
      if (tempIdx) {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = ArrayToken;
        strIdx += tempIdx;
        break;
      } else {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = IDToken;
        strIdx++;
        break;
      }
    case 'b':
      tempIdx = checkString((string + strIdx), "break");
      if (tempIdx) {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = BreakToken;
        strIdx += tempIdx;
        break;
      } else {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = IDToken;
        strIdx++;
        break;
      }
    case 'd':
      tempIdx = checkString((string + strIdx), "do");
      if (tempIdx) {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = DoToken;
        strIdx += tempIdx;
        break;
      } else {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = IDToken;
        strIdx++;
        break;
      }
    case 'e':
      tempIdx = checkString((string + strIdx), "else");
      if (tempIdx) {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = ElseToken;
        strIdx += tempIdx;
        break;
      }
      tempIdx = checkString((string + strIdx), "end");
      if (tempIdx) {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = EndToken;
        strIdx += tempIdx;
        break;
      } else {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = IDToken;
        strIdx++;
        break;
      }
    case 'f':
      tempIdx = checkString((string + strIdx), "for");
      if (tempIdx) {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = ForToken;
        strIdx += tempIdx;
        break;
      }
      tempIdx = checkString((string + strIdx), "function");
      if (tempIdx) {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = FunctionToken;
        strIdx += tempIdx;
        break;
      } else {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = IDToken;
        strIdx++;
        break;
      }
    case 'i':
      switch (string[strIdx + 1]) {
      case 'n':
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = InToken;
        strIdx += 2;
        break;
      case 'f':
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = IfToken;
        strIdx += 2;
        break;
      default:
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = IDToken;
        strIdx++;
        break;
      }
      break;
    case 'l':
      tempIdx = checkString((string + strIdx), "let");
      if (tempIdx) {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = LetToken;
        strIdx += tempIdx;
        break;
      } else {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = IDToken;
        strIdx++;
        break;
      }
    case 'n':
      tempIdx = checkString((string + strIdx), "nil");
      if (tempIdx) {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = NilToken;
        strIdx += tempIdx;
        break;
      } else {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = IDToken;
        strIdx++;
        break;
      }
    case 'o':
      tempIdx = checkString((string + strIdx), "of");
      if (tempIdx) {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = OfToken;
        strIdx += tempIdx;
        break;
      } else {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = IDToken;
        strIdx++;
        break;
      }
    case 't':
      tempIdx = checkString((string + strIdx), "then");
      if (tempIdx) {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = ThenToken;
        strIdx += tempIdx;
        break;
      }
      tempIdx = checkString((string + strIdx), "to");
      if (tempIdx) {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = ToToken;
        strIdx += tempIdx;
        break;
      }
      tempIdx = checkString((string + strIdx), "type");
      if (tempIdx) {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = TypeToken;
        strIdx += tempIdx;
        break;
      } else {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = IDToken;
        strIdx++;
        break;
      }
    case 'v':
      tempIdx = checkString((string + strIdx), "var");
      if (tempIdx) {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = VarToken;
        strIdx += tempIdx;
        break;
      } else {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = IDToken;
        strIdx++;
        break;
      }
    case 'w':
      tempIdx = checkString((string + strIdx), "while");
      if (tempIdx) {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = WhileToken;
        strIdx += tempIdx;
        break;
      } else {
        data[dataIdx++] = strIdx;
        tokens[tokIdx++] = IDToken;
        strIdx++;
        break;
      }
    default:
      data[dataIdx++] = strIdx;
      tokens[tokIdx++] = IDToken;
      strIdx++;
      break;
    }
  }
}
