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

#define MAX_STR_LEN 128

inline uchar eatInt(volatile __global char *strPtr,
                    volatile __global char *dataPtr) {
  int substrIdx = 0;
  char tmp[MAX_STR_LEN];
  while (substrIdx < MAX_STR_LEN) {
    char ch = strPtr[substrIdx];
    if (!(ch >= '0' && ch <= '9')) {
      break;
    }
    tmp[substrIdx > MAX_STR_LEN ? MAX_STR_LEN - 1 : substrIdx] = ch;
    substrIdx++;
  }
  tmp[substrIdx] = '\0';
  substrIdx = 0;
  while (substrIdx < 119) {
    dataPtr[substrIdx] = tmp[substrIdx];
    if (tmp[substrIdx] == '\0') {
      break;
    }
    substrIdx++;
  }

  return substrIdx;
}

inline uchar eatString(volatile __global char *strPtr,
                       volatile __global char *dataPtr) {
  int substrIdx = 0;
  while (strPtr[substrIdx] != '"') {
    dataPtr[substrIdx] = strPtr[substrIdx];
    substrIdx++;
  }
  dataPtr[substrIdx + 1] = '\0';
  return substrIdx;
}

inline uchar eatID(volatile __global char *strPtr,
                   volatile __global char *dataPtr) {
  int substrIdx = 0;
  char tmp[MAX_STR_LEN];
  while (substrIdx < MAX_STR_LEN) {
    char ch = strPtr[substrIdx];
    if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))) {
      break;
    }
    tmp[substrIdx > MAX_STR_LEN ? MAX_STR_LEN - 1 : substrIdx] = ch;
    substrIdx++;
  }
  tmp[substrIdx] = '\0';
  substrIdx = 0;
  while (substrIdx < 119) {
    dataPtr[substrIdx] = tmp[substrIdx];
    if (tmp[substrIdx] == '\0') {
      break;
    }
    substrIdx++;
  }

  return substrIdx;
}

inline uchar eatComment(volatile __global char *strPtr) {
  int substrIdx = 0;
  while (strPtr[substrIdx] != '*' || strPtr[substrIdx + 1] != '/') {
    substrIdx++;
  }
  return substrIdx + 2;
}

inline uchar checkString(volatile __global char *strPtr,
                         __constant const char *text) {
  int substrIdx = 0;
  while (text[substrIdx] != '\0') {
    if (strPtr[substrIdx] != text[substrIdx]) {
      return 0;
    }
    substrIdx++;
  }
  if (strPtr[substrIdx] == ' ' || strPtr[substrIdx] == '\n' ||
      strPtr[substrIdx] == '\r' || strPtr[substrIdx] == '\t') {
    return substrIdx;
  }
  return 0;
}

__kernel void lexer(__global char *string, __global char *tokens,
                    __global char *data, __global int *retVal) {
  int strIdx = 0;
  int dataIdx = 0;
  int tokIdx = 0;
  int tempIdx = 0;
  while (string[strIdx] != '\0') {
    if (strIdx > *(retVal)) {
      *(retVal) = -1;
      return;
    }
    if (dataIdx > *(retVal)) {
      *(retVal) = -2;
      return;
    }

    // This could potentially be moved into the main switch case, would probably
    // be more efficient.
    // Not anymore.. Lol :)
    if (string[strIdx] <= 32 || string[strIdx] >= 127) {
      strIdx++;
      continue;
    }

    switch (string[strIdx]) {
    case ',':
      tokens[tokIdx++] = CommaToken;
      strIdx++;
      break;
    case ';':
      tokens[tokIdx++] = SemicolonToken;
      strIdx++;
      break;
    case '(':
      tokens[tokIdx++] = LparenToken;
      strIdx++;
      break;
    case ')':
      tokens[tokIdx++] = RparenToken;
      strIdx++;
      break;
    case '[':
      tokens[tokIdx++] = LbrackToken;
      strIdx++;
      break;
    case ']':
      tokens[tokIdx++] = RbrackToken;
      strIdx++;
      break;
    case '{':
      tokens[tokIdx++] = LbraceToken;
      strIdx++;
      break;
    case '}':
      tokens[tokIdx++] = RbraceToken;
      strIdx++;
      break;
    case '.':
      tokens[tokIdx++] = DotToken;
      strIdx++;
      break;
    case '+':
      tokens[tokIdx++] = PlusToken;
      strIdx++;
      break;
    case '-':
      tokens[tokIdx++] = MinusToken;
      strIdx++;
      break;
    case '*':
      tokens[tokIdx++] = TimesToken;
      strIdx++;
      break;
    case '=':
      tokens[tokIdx++] = EqToken;
      strIdx++;
      break;
    case '&':
      tokens[tokIdx++] = AndToken;
      strIdx++;
      break;
    case '|':
      tokens[tokIdx++] = OrToken;
      strIdx++;
      break;
    case '^':
      tokens[tokIdx++] = CaretToken;
      strIdx++;
      break;
    // 2 length from here
    case '/':
      switch (string[strIdx + 1]) {
      case '*':
        strIdx += eatComment(string + strIdx + 2) + 2;
        break;
      default:
        tokens[tokIdx++] = DivideToken;
        strIdx++;
        break;
      }
      break;
    case '>':
      switch (string[strIdx + 1]) {
      case '=':
        tokens[tokIdx++] = GeToken;
        strIdx += 2;
        break;
      default:
        tokens[tokIdx++] = GtToken;
        strIdx++;
        break;
      }
      break;
    case '<':
      switch (string[strIdx + 1]) {
      case '=':
        tokens[tokIdx++] = LeToken;
        strIdx += 2;
        break;
      case '>':
        tokens[tokIdx++] = NeqToken;
        strIdx += 2;
        break;
      default:
        tokens[tokIdx++] = LtToken;
        strIdx++;
        break;
      }
      break;
    case ':':
      switch (string[strIdx + 1]) {
      case '=':
        tokens[tokIdx++] = AssignToken;
        strIdx += 2;
        break;
      default:
        tokens[tokIdx++] = ColonToken;
        strIdx++;
        break;
      }
      break;
    case 'a':
      tempIdx = checkString((string + strIdx), "array");
      if (tempIdx) {
        tokens[tokIdx++] = ArrayToken;
        strIdx += tempIdx;
        break;
      } else {
        goto default_label;
      }
    case 'b':
      tempIdx = checkString((string + strIdx), "break");
      if (tempIdx) {
        tokens[tokIdx++] = BreakToken;
        strIdx += tempIdx;
        break;
      } else {
        goto default_label;
      }
    case 'd':
      tempIdx = checkString((string + strIdx), "do");
      if (tempIdx) {
        tokens[tokIdx++] = DoToken;
        strIdx += tempIdx;
        break;
      } else {
        goto default_label;
      }
    case 'e':
      tempIdx = checkString((string + strIdx), "else");
      if (tempIdx) {
        tokens[tokIdx++] = ElseToken;
        strIdx += tempIdx;
        break;
      }
      tempIdx = checkString((string + strIdx), "end");
      if (tempIdx) {
        tokens[tokIdx++] = EndToken;
        strIdx += tempIdx;
        break;
      } else {
        goto default_label;
      }
    case 'f':
      tempIdx = checkString((string + strIdx), "for");
      if (tempIdx) {
        tokens[tokIdx++] = ForToken;
        strIdx += tempIdx;
        break;
      }
      tempIdx = checkString((string + strIdx), "function");
      if (tempIdx) {
        tokens[tokIdx++] = FunctionToken;
        strIdx += tempIdx;
        break;
      } else {
        goto default_label;
      }
    case 'i':
      tempIdx = checkString((string + strIdx), "in");
      if (tempIdx) {
        tokens[tokIdx++] = InToken;
        strIdx += tempIdx;
        break;
      }
      tempIdx = checkString((string + strIdx), "if");
      if (tempIdx) {
        tokens[tokIdx++] = IfToken;
        strIdx += tempIdx;
        break;
      } else {
        goto default_label;
      }
      break;
    case 'l':
      tempIdx = checkString((string + strIdx), "let");
      if (tempIdx) {
        tokens[tokIdx++] = LetToken;
        strIdx += tempIdx;
        break;
      } else {
        goto default_label;
      }
      break;
    case 'n':
      tempIdx = checkString((string + strIdx), "nil");
      if (tempIdx) {
        tokens[tokIdx++] = NilToken;
        strIdx += tempIdx;
        break;
      } else {
        goto default_label;
      }
    case 'o':
      tempIdx = checkString((string + strIdx), "of");
      if (tempIdx) {
        tokens[tokIdx++] = OfToken;
        strIdx += tempIdx;
        break;
      } else {
        goto default_label;
      }
    case 't':
      tempIdx = checkString((string + strIdx), "then");
      if (tempIdx) {
        tokens[tokIdx++] = ThenToken;
        strIdx += tempIdx;
        break;
      }
      tempIdx = checkString((string + strIdx), "to");
      if (tempIdx) {
        tokens[tokIdx++] = ToToken;
        strIdx += tempIdx;
        break;
      }
      tempIdx = checkString((string + strIdx), "type");
      if (tempIdx) {
        tokens[tokIdx++] = TypeToken;
        strIdx += tempIdx;
        break;
      } else {
        goto default_label;
      }
    case 'v':
      tempIdx = checkString((string + strIdx), "var");
      if (tempIdx) {
        tokens[tokIdx++] = VarToken;
        strIdx += tempIdx;
        break;
      } else {
        goto default_label;
      }
    case 'w':
      tempIdx = checkString((string + strIdx), "while");
      if (tempIdx) {
        tokens[tokIdx++] = WhileToken;
        strIdx += tempIdx;
        break;
      } else {
        goto default_label;
      }
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      tempIdx = eatInt((string + strIdx), (data + dataIdx));
      tokens[tokIdx++] = IntToken;
      dataIdx += tempIdx + 1;
      strIdx += tempIdx;
      break;
    case '"':
      strIdx++;
      tempIdx = eatString((string + strIdx), (data + dataIdx));
      tokens[tokIdx++] = StringToken;
      dataIdx += tempIdx + 1;
      strIdx += tempIdx + 1;
      break;
    default:
    default_label: // :)
      tempIdx = eatID((string + strIdx), (data + dataIdx));
      tokens[tokIdx++] = IDToken;
      dataIdx += tempIdx + 1;
      strIdx += tempIdx;
      break;
    }
  }
  tokens[tokIdx] = 0;
  *(retVal) = tokIdx;
}
