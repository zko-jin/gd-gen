#pragma once

#include <filesystem>
#include <fstream>
#include <queue>
#include <string>

enum class GToken
{
    Invalid = 0,
    GFUNCTION,
    Struct,
    GPROPERTY,
    GCLASS,
    GSTRUCT,
    GENUM,
    GSIGNAL,
    Class,
    Identifier,
    Integer,
    Float,
    Equal,
    Const,
    Mutable,
    Pointer,
    LeftParenthesis,
    RightParenthesis,
    LeftCurlyBrace,
    RightCurlyBrace,
    Colon,
    String,
    Asterisk,
    Comma,
    GENERATED_BODY,
    Public,
    Enum,
    Private,
    Protected,
    Inline,
    Static,
    Virtual
};

std::string GTokenToString(GToken token);

struct TokenValue
{
    GToken token;
    std::string value;
    unsigned int line;
};

class TokenStream
{
    std::queue<TokenValue> tokens;
    std::string filename;

   public:
    TokenStream(std::queue<TokenValue> tokens, std::string filename)
    {
        this->tokens = tokens;
        this->filename = filename;
    }

    TokenValue next()
    {
        TokenValue token = tokens.front();
        tokens.pop();
        return token;
    }

    bool empty() { return tokens.empty(); }

    std::string get_filename() { return this->filename; }
};

class Parser
{
    unsigned int cur_line = 1;
    inline void ignore_until_newline();
    inline void ignore_until_end_comment();
    inline TokenValue read_identifier();
    inline TokenValue read_number();
    inline TokenValue read_string();
    std::ifstream file;
    std::string file_name;

   public:
    Parser(std::filesystem::path path)
    {
        this->file_name = path.filename().string();
        this->file = std::ifstream(path);
    }

    ~Parser() { this->file.close(); }

    TokenStream parse();
};