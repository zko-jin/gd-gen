#include "lexer.h"

#include <helpers/logger.h>

enum class SpecialChars
{
    Empty = 0,
    Something,
    Digit,
    NewLine,
    SingleQuote,
    DoubleQuote,
    LeftParenthesis,
    RightParenthesis,
    LeftCurlyBrace,
    RightCurlyBrace,
    Asterisk,
    Comma,
    Slash,
    Period,
    Colon,
    Equal
};

const SpecialChars special_chars[255] = {
    // Control characters (0–31)
    SpecialChars::Empty,    // 0: NUL (Null character)
    SpecialChars::Empty,    // 1: SOH (Start of Header)
    SpecialChars::Empty,    // 2: STX (Start of Text)
    SpecialChars::Empty,    // 3: ETX (End of Text)
    SpecialChars::Empty,    // 4: EOT (End of Transmission)
    SpecialChars::Empty,    // 5: ENQ (Enquiry)
    SpecialChars::Empty,    // 6: ACK (Acknowledge)
    SpecialChars::Empty,    // 7: BEL (Bell)
    SpecialChars::Empty,    // 8: BS  (Backspace)
    SpecialChars::Empty,    // 9: TAB (Horizontal Tab)
    SpecialChars::NewLine,  // 10: LF (Line Feed, Newline)
    SpecialChars::Empty,    // 11: VT (Vertical Tab)
    SpecialChars::Empty,    // 12: FF (Form Feed)
    SpecialChars::Empty,    // 13: CR (Carriage Return)
    SpecialChars::Empty,    // 14: SO (Shift Out)
    SpecialChars::Empty,    // 15: SI (Shift In)
    SpecialChars::Empty,    // 16: DLE (Data Link Escape)
    SpecialChars::Empty,    // 17: DC1 (Device Control 1)
    SpecialChars::Empty,    // 18: DC2 (Device Control 2)
    SpecialChars::Empty,    // 19: DC3 (Device Control 3)
    SpecialChars::Empty,    // 20: DC4 (Device Control 4)
    SpecialChars::Empty,    // 21: NAK (Negative Acknowledge)
    SpecialChars::Empty,    // 22: SYN (Synchronous Idle)
    SpecialChars::Empty,    // 23: ETB (End of Transmission Block)
    SpecialChars::Empty,    // 24: CAN (Cancel)
    SpecialChars::Empty,    // 25: EM  (End of Medium)
    SpecialChars::Empty,    // 26: SUB (Substitute)
    SpecialChars::Empty,    // 27: ESC (Escape)
    SpecialChars::Empty,    // 28: FS  (File Separator)
    SpecialChars::Empty,    // 29: GS  (Group Separator)
    SpecialChars::Empty,    // 30: RS  (Record Separator)
    SpecialChars::Empty,    // 31: US  (Unit Separator)

    // Printable characters (32–126)
    SpecialChars::Empty,             // 32: ' ' (Space)
    SpecialChars::Empty,             // 33: '!' (Exclamation mark)
    SpecialChars::DoubleQuote,       // 34: '"' (Double quote)
    SpecialChars::Empty,             // 35: '#' (Hash)
    SpecialChars::Empty,             // 36: '$' (Dollar sign)
    SpecialChars::Empty,             // 37: '%' (Percent sign)
    SpecialChars::Empty,             // 38: '&' (Ampersand)
    SpecialChars::SingleQuote,       // 39: '\'' (Single quote)
    SpecialChars::LeftParenthesis,   // 40: '(' (Left parenthesis)
    SpecialChars::RightParenthesis,  // 41: ')' (Right parenthesis)
    SpecialChars::Asterisk,          // 42: '*' (Asterisk)
    SpecialChars::Empty,             // 43: '+' (Plus)
    SpecialChars::Comma,             // 44: ',' (Comma)
    SpecialChars::Empty,             // 45: '-' (Hyphen)
    SpecialChars::Period,            // 46: '.' (Period)
    SpecialChars::Slash,             // 47: '/' (Slash)
    SpecialChars::Digit,             // 48: '0' (Digit zero)
    SpecialChars::Digit,             // 49: '1' (Digit one)
    SpecialChars::Digit,             // 50: '2' (Digit two)
    SpecialChars::Digit,             // 51: '3' (Digit three)
    SpecialChars::Digit,             // 52: '4' (Digit four)
    SpecialChars::Digit,             // 53: '5' (Digit five)
    SpecialChars::Digit,             // 54: '6' (Digit six)
    SpecialChars::Digit,             // 55: '7' (Digit seven)
    SpecialChars::Digit,             // 56: '8' (Digit eight)
    SpecialChars::Digit,             // 57: '9' (Digit nine)
    SpecialChars::Colon,             // 58: ':' (Colon)
    SpecialChars::Empty,             // 59: ';' (Semicolon)
    SpecialChars::Something,         // 60: '<' (Less-than)
    SpecialChars::Equal,             // 61: '=' (Equal sign)
    SpecialChars::Something,         // 62: '>' (Greater-than)
    SpecialChars::Empty,             // 63: '?' (Question mark)
    SpecialChars::Empty,             // 64: '@' (At symbol)
    SpecialChars::Something,         // 65: 'A' (Uppercase A)
    SpecialChars::Something,         // 66: 'B' (Uppercase B)
    SpecialChars::Something,         // 67: 'C' (Uppercase C)
    SpecialChars::Something,         // 68: 'D' (Uppercase D)
    SpecialChars::Something,         // 69: 'E' (Uppercase E)
    SpecialChars::Something,         // 70: 'F' (Uppercase F)
    SpecialChars::Something,         // 71: 'G' (Uppercase G)
    SpecialChars::Something,         // 72: 'H' (Uppercase H)
    SpecialChars::Something,         // 73: 'I' (Uppercase I)
    SpecialChars::Something,         // 74: 'J' (Uppercase J)
    SpecialChars::Something,         // 75: 'K' (Uppercase K)
    SpecialChars::Something,         // 76: 'L' (Uppercase L)
    SpecialChars::Something,         // 77: 'M' (Uppercase M)
    SpecialChars::Something,         // 78: 'N' (Uppercase N)
    SpecialChars::Something,         // 79: 'O' (Uppercase O)
    SpecialChars::Something,         // 80: 'P' (Uppercase P)
    SpecialChars::Something,         // 81: 'Q' (Uppercase Q)
    SpecialChars::Something,         // 82: 'R' (Uppercase R)
    SpecialChars::Something,         // 83: 'S' (Uppercase S)
    SpecialChars::Something,         // 84: 'T' (Uppercase T)
    SpecialChars::Something,         // 85: 'U' (Uppercase U)
    SpecialChars::Something,         // 86: 'V' (Uppercase V)
    SpecialChars::Something,         // 87: 'W' (Uppercase W)
    SpecialChars::Something,         // 88: 'X' (Uppercase X)
    SpecialChars::Something,         // 89: 'Y' (Uppercase Y)
    SpecialChars::Something,         // 90: 'Z' (Uppercase Z)
    SpecialChars::Empty,             // 91: '[' (Left square bracket)
    SpecialChars::Empty,             // 92: '\\' (Backslash)
    SpecialChars::Empty,             // 93: ']' (Right square bracket)
    SpecialChars::Empty,             // 94: '^' (Caret)
    SpecialChars::Something,         // 95: '_' (Underscore)
    SpecialChars::Empty,             // 96: '`' (Grave accent)
    SpecialChars::Something,         // 97: 'a' (Lowercase a)
    SpecialChars::Something,         // 98: 'b' (Lowercase b)
    SpecialChars::Something,         // 99: 'c' (Lowercase c)
    SpecialChars::Something,         // 100: 'd' (Lowercase d)
    SpecialChars::Something,         // 101: 'e' (Lowercase e)
    SpecialChars::Something,         // 102: 'f' (Lowercase f)
    SpecialChars::Something,         // 103: 'g' (Lowercase g)
    SpecialChars::Something,         // 104: 'h' (Lowercase h)
    SpecialChars::Something,         // 105: 'i' (Lowercase i)
    SpecialChars::Something,         // 106: 'j' (Lowercase j)
    SpecialChars::Something,         // 107: 'k' (Lowercase k)
    SpecialChars::Something,         // 108: 'l' (Lowercase l)
    SpecialChars::Something,         // 109: 'm' (Lowercase m)
    SpecialChars::Something,         // 110: 'n' (Lowercase n)
    SpecialChars::Something,         // 111: 'o' (Lowercase o)
    SpecialChars::Something,         // 112: 'p' (Lowercase p)
    SpecialChars::Something,         // 113: 'q' (Lowercase q)
    SpecialChars::Something,         // 114: 'r' (Lowercase r)
    SpecialChars::Something,         // 115: 's' (Lowercase s)
    SpecialChars::Something,         // 116: 't' (Lowercase t)
    SpecialChars::Something,         // 117: 'u' (Lowercase u)
    SpecialChars::Something,         // 118: 'v' (Lowercase v)
    SpecialChars::Something,         // 119: 'w' (Lowercase w)
    SpecialChars::Something,         // 120: 'x' (Lowercase x)
    SpecialChars::Something,         // 121: 'y' (Lowercase y)
    SpecialChars::Something,         // 122: 'z' (Lowercase z)
    SpecialChars::LeftCurlyBrace,    // 123: '{' (Left curly brace)
    SpecialChars::Empty,             // 124: '|' (Vertical bar)
    SpecialChars::RightCurlyBrace,   // 125: '}' (Right curly brace)
    SpecialChars::Empty,             // 126: '~' (Tilde)
    SpecialChars::Empty              // 127: DEL (Delete)
};

inline void Parser::ignore_until_newline()
{
    char c;
    while (file.get(c))
    {
        if (c == '\n')
        {
            cur_line++;
            break;
        }
    }
}

inline void Parser::ignore_until_end_comment()
{
    char c;
    while (file.get(c))
    {
        if (c == '*' && file.peek() == '/')
        {
            break;
        }
        else if (c == '\n')
        {
            cur_line++;
        }
    }
}

inline TokenValue Parser::read_string()
{
    std::string str;
    char c;
    while (file.get(c))
    {
        if (c == '"' || c == '\'')
        {
            return TokenValue{
                GToken::String,
                str,
                cur_line,
            };
        }
        else
        {
            str.push_back(c);
        }
    }

    Logger::log("Unterminated string", LogLevel::Error, file_name, cur_line);
    exit(1);
}

inline TokenValue Parser::read_identifier()
{
    std::string str;
    char c;
    while (file.get(c))
    {
        if (special_chars[c] == SpecialChars::Something || special_chars[c] == SpecialChars::Digit)
        {
            str.push_back(c);
        }
        else
        {
            file.putback(c);
            if (str == "GPROPERTY")
            {
                return TokenValue{
                    GToken::GPROPERTY,
                    str,
                    cur_line,
                };
            }
            else if (str == "GCLASS")
            {
                return TokenValue{
                    GToken::GCLASS,
                    str,
                    cur_line,
                };
            }
            else if (str == "GSTRUCT")
            {
                return TokenValue{
                    GToken::GSTRUCT,
                    str,
                    cur_line,
                };
            }
            else if (str == "GSIGNAL")
            {
                return TokenValue{
                    GToken::GSIGNAL,
                    str,
                    cur_line,
                };
            }
            else if (str == "GFUNCTION")
            {
                return TokenValue{
                    GToken::GFUNCTION,
                    str,
                    cur_line,
                };
            }
            else if (str == "class")
            {
                return TokenValue{
                    GToken::Class,
                    str,
                    cur_line,
                };
            }
            else if (str == "struct")
            {
                return TokenValue{
                    GToken::Struct,
                    str,
                    cur_line,
                };
            }
            else if (str == "const")
            {
                return TokenValue{
                    GToken::Const,
                    str,
                    cur_line,
                };
            }
            else if (str == "mutable")
            {
                return TokenValue{
                    GToken::Mutable,
                    str,
                    cur_line,
                };
            }
            else if (str == "GENERATED_BODY")
            {
                return TokenValue{
                    GToken::GENERATED_BODY,
                    str,
                    cur_line,
                };
            }
            else if (str == "GENUM")
            {
                return TokenValue{
                    GToken::GENUM,
                    str,
                    cur_line,
                };
            }
            else if (str == "enum")
            {
                return TokenValue{
                    GToken::Enum,
                    str,
                    cur_line,
                };
            }
            else if (str == "public")
            {
                return TokenValue{
                    GToken::Public,
                    str,
                    cur_line,
                };
            }
            else if (str == "protected")
            {
                return TokenValue{
                    GToken::Protected,
                    str,
                    cur_line,
                };
            }
            else if (str == "private")
            {
                return TokenValue{
                    GToken::Private,
                    str,
                    cur_line,
                };
            }
            else if (str == "inline")
            {
                return TokenValue{
                    GToken::Inline,
                    str,
                    cur_line,
                };
            }
            else if (str == "static")
            {
                return TokenValue{
                    GToken::Static,
                    str,
                    cur_line,
                };
            }
            else if (str == "virtual")
            {
                return TokenValue{
                    GToken::Virtual,
                    str,
                    cur_line,
                };
            }
            else
            {
                break;
            }
        }
    }

    return TokenValue{
        GToken::Identifier,
        str,
        cur_line,
    };
}

inline TokenValue Parser::read_number()
{
    std::string str;
    GToken token_type = GToken::Integer;
    char c;
    while (file.get(c))
    {
        if (special_chars[c] == SpecialChars::Digit)
        {
            str.push_back(c);
        }
        else if (special_chars[c] == SpecialChars::Period)
        {
            token_type = GToken::Float;
            str.push_back(c);
        }
        else if (c == 'f')
        {
            token_type = GToken::Float;
            break;
        }
        else
        {
            file.putback(c);
            break;
        }
    }

    return TokenValue{
        token_type,
        str,
        cur_line,
    };
}

TokenStream Parser::parse()
{
    std::queue<TokenValue> tokens;
    std::string value;
    char c;

    while (file.get(c))
    {
        char peek;
        switch (special_chars[c])
        {
            case SpecialChars::Something:
                file.putback(c);
                tokens.push(read_identifier());
                break;
            case SpecialChars::SingleQuote:
            case SpecialChars::DoubleQuote: tokens.push(read_string()); break;
            case SpecialChars::Digit:
                file.putback(c);
                tokens.push(read_number());
                break;
            case SpecialChars::Slash:
                peek = file.peek();
                if (peek == '/')
                {
                    ignore_until_newline();
                }
                else if (peek == '*')
                {
                    ignore_until_end_comment();
                }
                break;
            case SpecialChars::Asterisk:
                tokens.push(TokenValue{
                    GToken::Asterisk,
                    "*",
                });
                break;
            case SpecialChars::Comma:
                tokens.push(TokenValue{
                    GToken::Comma,
                    ",",
                });
                break;
            case SpecialChars::LeftParenthesis:
                tokens.push(TokenValue{
                    GToken::LeftParenthesis,
                    "(",
                });
                break;
            case SpecialChars::RightParenthesis:
                tokens.push(TokenValue{
                    GToken::RightParenthesis,
                    ")",
                });
                break;
            case SpecialChars::LeftCurlyBrace:
                tokens.push(TokenValue{
                    GToken::LeftCurlyBrace,
                    "{",
                });
                break;
            case SpecialChars::RightCurlyBrace:
                tokens.push(TokenValue{
                    GToken::RightCurlyBrace,
                    "}",
                });
                break;
            case SpecialChars::Colon:
                tokens.push(TokenValue{
                    GToken::Colon,
                    ":",
                });
                break;
            case SpecialChars::NewLine: cur_line++; break;
            case SpecialChars::Equal:
                tokens.push(TokenValue{
                    GToken::Equal,
                    "=",
                });
                break;
        }
    }
    return TokenStream(tokens, file_name);
}

std::string GTokenToString(GToken token)
{
    switch (token)
    {
        case GToken::Invalid: return "Invalid";
        case GToken::GFUNCTION: return "GFUNCTION";
        case GToken::GPROPERTY: return "GPROPERTY";
        case GToken::GCLASS: return "GCLASS";
        case GToken::Class: return "Class";
        case GToken::Identifier: return "Identifier";
        case GToken::Const: return "Const";
        case GToken::Pointer: return "Pointer";
        case GToken::LeftParenthesis: return "LeftParenthesis";
        case GToken::Float: return "Float";
        case GToken::RightParenthesis: return "RightParenthesis";
        case GToken::LeftCurlyBrace: return "LeftCurlyBrace";
        case GToken::RightCurlyBrace: return "RightCurlyBrace";
        case GToken::Colon: return "Colon";
        case GToken::String: return "String";
        case GToken::Asterisk: return "Asterisk";
        case GToken::Comma: return "Comma";
        case GToken::GENERATED_BODY: return "GENERATED_BODY";
        case GToken::Public: return "Public";
        case GToken::Private: return "Private";
        case GToken::Protected: return "Protected";
        case GToken::Integer: return "Number";
        case GToken::Equal: return "Equal";
        case GToken::Struct: return "Struct";
        case GToken::GSTRUCT: return "GSTRUCT";
        case GToken::GENUM: return "GENUM";
        case GToken::GSIGNAL: return "GSIGNAL";
        case GToken::Enum: return "Enum";
        case GToken::Inline: return "Inline";
        default: return "Unknown";
    }
}