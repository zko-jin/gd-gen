#include "GSignal.h"

#include <helpers/logger.h>

GSignalOptions::GSignalOptions(TokenStream &token_stream)
{
    TokenValue token;
    bool not_first = false;

    // Reading Options
    while (!token_stream.empty())
    {
        token = token_stream.next();

        if (token.token == GToken::RightParenthesis)
        {
            if (!not_first)
            {
                Logger::log("GSignal options expected at least one option due to comma",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }
            break;
        }

        if (not_first)
        {
            if (token.token != GToken::Comma)
            {
                Logger::log("GSignal options expected ',', got '" + token.value + "'",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }
            token = token_stream.next();
        }

        if (token.token != GToken::Identifier)
        {
            Logger::log("GSignal options expected identifier, got '" + token.value + "'",
                        LogLevel::Error, token_stream.get_filename(), token.line);
            exit(1);
        }

        not_first = true;
    }
}

GSignal::GSignal(TokenStream &token_stream)
{
    TokenValue token = token_stream.next();

    if (token.token != GToken::LeftParenthesis)
    {
        Logger::log("GSignal expected left parenthesis, got '" + token.value + "'", LogLevel::Error,
                    token_stream.get_filename(), token.line);
        exit(1);
    }

    token = token_stream.next();
    if (token.token != GToken::Identifier)
    {
        Logger::log("GSignal expected an identifier for name, got '" + token.value + "'",
                    LogLevel::Error, token_stream.get_filename(), token.line);
        exit(1);
    }
    name = token.value;

    arguments = GArgument::read_garguments(token_stream);

    for (auto& argument : arguments)
    {
        if (!argument.value.empty())
        {
            Logger::log("GSignal does not support default arguments, for signal '" 
                + name + "', argument '" + argument.name + "'",
                    LogLevel::Error, token_stream.get_filename(), token.line);
            exit(1);
        }
    }
    
    token = token_stream.next();

    if (token.token == GToken::RightParenthesis)
    {
        return;
    }

    if (token.token != GToken::Comma)
    {
        Logger::log("GSignal expected ',', got '" + token.value + "'", LogLevel::Error,
                    token_stream.get_filename(), token.line);
        exit(1);
    }

    options = GSignalOptions(token_stream);
}
