#include "GClass.h"

#include <helpers/logger.h>

GClassOptions::GClassOptions(TokenStream &token_stream)
{
    TokenValue token = token_stream.next();

    if (token.token != GToken::LeftParenthesis)
    {
        Logger::log("Options expected left parenthesis, got '" + token.value + "'", LogLevel::Error,
                    token_stream.get_filename(), token.line);
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
                Logger::log("GClass options expected ','", LogLevel::Error,
                            token_stream.get_filename(), token.line);
                exit(1);
            }
            token = token_stream.next();
        }

        if (token.token != GToken::Identifier)
        {
            Logger::log("GClass options expected identifier, got '" + token.value + "'",
                        LogLevel::Error, token_stream.get_filename(), token.line);
            exit(1);
        }

        if (token.value == "Resource")
        {
            is_resource = true;
        }
        else if (token.value == "CustomBindings")
        {
            custom_bindings = true;
        }
        else if (token.value == "Virtual")
        {
            if (class_type != GClassType::Base)
            {
                Logger::log("Class can't be " + get_class_type_string(GClassType::Virtual) +
                                " and " + get_class_type_string(class_type) + " at the same time",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }
            class_type = GClassType::Virtual;
        }
        else if (token.value == "Abstract")
        {
            if (class_type != GClassType::Base)
            {
                Logger::log("Class can't be " + get_class_type_string(GClassType::Abstract) +
                                " and " + get_class_type_string(class_type) + " at the same time",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }
            class_type = GClassType::Abstract;
        }
        else if (token.value == "Runtime")
        {
            if (class_type != GClassType::Base)
            {
                Logger::log("Class can't be " + get_class_type_string(GClassType::Runtime) +
                                " and " + get_class_type_string(class_type) + " at the same time",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }
            class_type = GClassType::Runtime;
        }
        else if (token.value == "Internal")
        {
            if (class_type != GClassType::Base)
            {
                Logger::log("Class can't be " + get_class_type_string(GClassType::Internal) +
                                " and " + get_class_type_string(class_type) + " at the same time",
                            LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }
            class_type = GClassType::Internal;
        }
        else
        {
            Logger::log("Unknown GClass option '" + token.value + "'", LogLevel::Error,
                        token_stream.get_filename(), token.line);
            exit(1);
        }

        not_first = true;
    }
}

GClass::GClass(TokenStream &token_stream)
{
    options = GClassOptions(token_stream);

    TokenValue token = token_stream.next();

    if (token.token != GToken::Class)
    {
        Logger::log("GClass expected 'class', got '" + token.value + "'", LogLevel::Error,
                    token_stream.get_filename(), token.line);
        exit(1);
    }

    token = token_stream.next();
    
    //ignore export macros
    if (token.token == GToken::Identifier && token.value == "GDE_EXPORT")
        token = token_stream.next();
    
    if (token.token != GToken::Identifier)
    {
        Logger::log("GClass expected an identifier for the class name, got '" + token.value + "'",
                    LogLevel::Error, token_stream.get_filename(), token.line);
        exit(1);
    }
    name = token.value;

    token = token_stream.next();
    if (token.token != GToken::Colon)
    {
        Logger::log("GClass expected ':' as every GClass needs to inherit something, got '" +
                        token.value + "'",
                    LogLevel::Error, token_stream.get_filename(), token.line);
        exit(1);
    }

    token = token_stream.next();
    if (token.token != GToken::Public)
    {
        Logger::log("GClass expected 'public' access modifier, got '" + token.value + "'",
                    LogLevel::Error, token_stream.get_filename(), token.line);
        exit(1);
    }

    token = token_stream.next();
    if (token.token != GToken::Identifier)
    {
        Logger::log(
            "GClass expected an identifier for the parent class name, got '" + token.value + "'",
            LogLevel::Error, token_stream.get_filename(), token.line);
        exit(1);
    }
    parentName = token.value;
}

std::string get_class_type_string(GClassType type)
{
    switch (type)
    {
        case GClassType::Base: return "Base";
        case GClassType::Virtual: return "Virtual";
        case GClassType::Abstract: return "Abstract";
        case GClassType::Runtime: return "Runtime";
    }
    return "";
}
