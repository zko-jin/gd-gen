#pragma once

#include <core/general/GType.h>
#include <lexer/lexer.h>

#include <queue>
#include <string>
#include <vector>

struct GArgument
{
    GType variantType = GType::Invalid;
    std::string raw_type;
    std::string name;
    std::string value;
    
    bool isPointer = false;
    bool isConst = false;

    static std::vector<GArgument> read_garguments(TokenStream &token_stream);
};