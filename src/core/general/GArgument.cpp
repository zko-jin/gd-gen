#include "GArgument.h"

#include <helpers/logger.h>

#include <iostream>

std::vector<GArgument> GArgument::read_garguments(TokenStream &token_stream)
{
    std::vector<GArgument> arguments;
    TokenValue token = token_stream.next();

    if (token.token != GToken::LeftParenthesis)
    {
        Logger::log("GArguments expected left parenthesis got '" + token.value + "'",
                    LogLevel::Error, token_stream.get_filename(), token.line);
        exit(1);
    }

    while (!token_stream.empty())
    {
        if (token.token == GToken::RightParenthesis)
        {
            break;
        }
        
        if (arguments.size() > 0)
        {
            if (token.token != GToken::Comma)
            {
                Logger::log("GArguments expected ',' got '" + token.value + "'", LogLevel::Error,
                            token_stream.get_filename(), token.line);
                exit(1);
            }
            token = token_stream.next();
        }
        else
            token = token_stream.next();

        if (token.token == GToken::RightParenthesis)
        {
            break;
        }

        GArgument gArgument = {};
        
        if (token.token == GToken::Const)
        {
            token = token_stream.next();
            gArgument.isConst = true;
        }

        if (token.token != GToken::Identifier)
        {
            Logger::log("GArguments expected identifier for type, got '" + token.value + "'",
                        LogLevel::Error, token_stream.get_filename(), token.line);
            exit(1);
        }
        gArgument.raw_type = token.value;

        // TODO: This is copied from GPROPERTY, create a common function
        token = token_stream.next();
        if (token.token == GToken::Asterisk)
        {
            gArgument.variantType = GType::NodePathToRaw;
            token = token_stream.next();
        }
        else if (gArgument.raw_type == "float" || gArgument.raw_type == "double")
        {
            gArgument.variantType = GType::Float;
        }
        else if (gArgument.raw_type == "int")
        {
            gArgument.variantType = GType::Int;
        }
        else if (gArgument.raw_type == "bool")
        {
            gArgument.variantType = GType::Boolean;
        }
        else if (gArgument.raw_type == "String")
        {
            gArgument.variantType = GType::String;
        }
        else if (gArgument.raw_type == "PackedByteArray")
        {
            gArgument.variantType = GType::PackedByteArray;
        }
        else if (gArgument.raw_type == "PackedInt32Array")
        {
            gArgument.variantType = GType::PackedInt32Array;
        }
        else if (gArgument.raw_type == "PackedInt64Array")
        {
            gArgument.variantType = GType::PackedInt64Array;
        }
        else if (gArgument.raw_type == "PackedFloat32Array")
        {
            gArgument.variantType = GType::PackedFloat32Array;
        }
        else if (gArgument.raw_type == "PackedFloat64Array")
        {
            gArgument.variantType = GType::PackedFloat64Array;
        }
        else if (gArgument.raw_type == "PackedStringArray")
        {
            gArgument.variantType = GType::PackedStringArray;
        }
        else if (gArgument.raw_type == "PackedVector2Array")
        {
            gArgument.variantType = GType::PackedVector2Array;
        }
        else if (gArgument.raw_type == "PackedVector3Array")
        {
            gArgument.variantType = GType::PackedVector3Array;
        }
        else if (gArgument.raw_type == "PackedColorArray")
        {
            gArgument.variantType = GType::PackedColorArray;
        }
        else if (gArgument.raw_type == "PackedVector4Array")
        {
            gArgument.variantType = GType::PackedVector4Array;
        }
        else if (gArgument.raw_type.starts_with("Ref<"))
        {
            gArgument.variantType = GType::Resource;
        }
        else
        {
            gArgument.variantType = GType::Object;
        }

        if (token.token != GToken::Identifier)
        {
            Logger::log("GArguments expected identifier after type, got '" + token.value + "'",
                        LogLevel::Error, token_stream.get_filename(), token.line);
            exit(1);
        }
        gArgument.name = token.value;

        token = token_stream.next();
        
        if (token.token == GToken::Equal)
        {
            token = token_stream.next();
            if (token.token == GToken::Comma && token.token == GToken::RightParenthesis)
            {
                Logger::log("GArguments expected value after =, got '" + token.value + "'",
                        LogLevel::Error, token_stream.get_filename(), token.line);
                exit(1);
            }
            
            gArgument.value = token.value;
            
            token = token_stream.next();
        }
        
        arguments.push_back(gArgument);
    }

    return arguments;
}