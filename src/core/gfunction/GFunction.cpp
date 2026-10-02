#include "GFunction.h"

#include <helpers/logger.h>

GFunctionOptions::GFunctionOptions(TokenStream &token_stream)
{
    TokenValue token = token_stream.next();

    if (token.token != GToken::LeftParenthesis)
    {
        Logger::log("GFunction options expected left parenthesis, got '" + token.value + "'",
                    LogLevel::Error, token_stream.get_filename(), token.line);
        exit(1);
    }

    bool not_first = false;

    // Reading Options
    while (!token_stream.empty())
    {
        token = token_stream.next();

        if (token.token == GToken::RightParenthesis)
        {
            break;
        }

        if (not_first)
        {
            if (token.token != GToken::Comma)
            {
                Logger::log("GFunction options expected ',', got " + token.value, LogLevel::Error,
                            token_stream.get_filename(), token.line);
                exit(1);
            }
            token = token_stream.next();
        }

        if (token.token != GToken::Identifier)
        {
            Logger::log("GFunction options expected identifier, got '" + token.value + "'",
                        LogLevel::Error, token_stream.get_filename(), token.line);
            exit(1);
        }

        not_first = true;
    }
}

GFunction::GFunction(TokenStream &token_stream)
{
    options = GFunctionOptions(token_stream);

    TokenValue token;

    while (true)
    {
        token = token_stream.next();

        if (token.token == GToken::Inline)
        {
            isInline = true;
        }
        else if (token.token == GToken::Const)
        {
            isConst = true;
        }
        else if (token.token == GToken::Static)
        {
            isStatic = true;
        }
        else if (token.token == GToken::Virtual)
        {
            isVirtual = true;
        }
        else if (token.token == GToken::Identifier)
        {
            returnType = token.value;
            break;
        }
        else
        {
            Logger::log(
                "GFunction expected an identifier for return type, got '" + token.value + "'",
                LogLevel::Error, token_stream.get_filename(), token.line);
            exit(1);
        }
    }

    token = token_stream.next();
    if (token.token == GToken::Asterisk)
    {
        //Is pointer type
        returnType += "*";
        
        token = token_stream.next();
    }
    if (token.token != GToken::Identifier)
    {
        Logger::log(
            "GFunction expected an identifier for the function name, got '" + token.value + "'",
            LogLevel::Error, token_stream.get_filename(), token.line);
        exit(1);
    }
    name = token.value;

    arguments = GArgument::read_garguments(token_stream);
}
