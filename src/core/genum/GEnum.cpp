#include "GEnum.h"

#include <helpers/logger.h>

GEnumOptions::GEnumOptions(TokenStream &token_stream)
{
    TokenValue token = token_stream.next();

    if (token.token != GToken::LeftParenthesis)
    {
        Logger::log("GEnum options expected left parenthesis, got '" + token.value + "'",
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
                Logger::log("GEnum options expected ','", LogLevel::Error,
                            token_stream.get_filename(), token.line);
                exit(1);
            }
            token = token_stream.next();
        }

        if (token.token != GToken::Identifier)
        {
            Logger::log("GEnum options expected identifier, got '" + token.value + "'",
                        LogLevel::Error, token_stream.get_filename(), token.line);
            exit(1);
        }

        not_first = true;
    }
}

GEnum::GEnum(TokenStream &token_stream)
{
    options = GEnumOptions(token_stream);

    TokenValue token = token_stream.next();

    if (token.token != GToken::Enum)
    {
        Logger::log("GEnum expected 'enum', got '" + token.value + "'", LogLevel::Error,
                    token_stream.get_filename(), token.line);
        exit(1);
    }

    token = token_stream.next();

    if (token.token == GToken::Class)
    {
        token = token_stream.next();
    }

    if (token.token != GToken::Identifier)
    {
        Logger::log("GEnum expected an identifier, got '" + token.value + "'", LogLevel::Error,
                    token_stream.get_filename(), token.line);
        exit(1);
    }
    name = token.value;

    token = token_stream.next();

    if (token.token != GToken::LeftCurlyBrace)
    {
        Logger::log("GEnum expected '{', got '" + token.value + "'", LogLevel::Error,
                    token_stream.get_filename(), token.line);
        exit(1);
    }

    
    while (!token_stream.empty())
    {
        token = token_stream.next();
        GEnum::EnumValue enumValue = {};

        if (token.token == GToken::RightCurlyBrace)
        {
            break;
        }

        if (values.size() > 0)
        {
            if (token.token != GToken::Comma)
            {
                Logger::log("GEnum expected ',', got '" + token.value + "'", LogLevel::Error,
                            token_stream.get_filename(), token.line);
                exit(1);
            }
            token = token_stream.next();

            if (token.token == GToken::RightCurlyBrace)
            {
                break;
            }
        }

        if (token.token != GToken::Identifier)
        {
            Logger::log("GEnum expected an identifier, got '" + token.value + "'", LogLevel::Error,
                        token_stream.get_filename(), token.line);
            exit(1);
        }
        enumValue.name = token.value;

        token = token_stream.next();

        if (token.token == GToken::Equal)
        {
            token = token_stream.next();

            if (token.token != GToken::Integer)
            {
                Logger::log("GEnum expected an integer, got '" + token.value + "'", LogLevel::Error,
                            token_stream.get_filename(), token.line);
                exit(1);
            }
            enumValue.value = std::stoi(token.value);
        }

        values.push_back(enumValue);
    }
}
