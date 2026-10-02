#pragma once

#include "../../macro_helpers.hpp"

constexpr bool is_valid_gproperty_flag(const char *flag)
{
    constexpr const char *valid_flags[] = {"Required", "NoStorage", "HideInInspector",
                                           "MultilineText", "ReadOnly", "Untyped"};
    constexpr size_t num_valid = sizeof(valid_flags) / sizeof(valid_flags[0]);

    for (size_t i = 0; i < num_valid; ++i)
    {
        if (str_equal(flag, valid_flags[i])) return true;
    }
    if (starts_with(flag, "Description("))  // TODO: Validate string
        return true;

    if (starts_with(flag, "ShowIf("))  // TODO: Validate statement
        return true;

    if (starts_with(flag, "Getter("))  // TODO: Validate statement
        return true;

    if (starts_with(flag, "Setter("))  // TODO: Validate statement
        return true;

    if (starts_with(flag, "Range("))  // TODO: Validate values
        return true;

    if (starts_with(flag, "Group("))  // TODO: Validate value
        return true;

    // if (starts_with(flag, "EditableIf("))  // TODO: Validate value
    //     return true;

    return false;
}

#define CHECK_GPROPERTY_FLAG(flag) \
    static_assert(is_valid_gproperty_flag(#flag), "Invalid GPROPERTY flag: " #flag);

#define VALIDATE_EACH_GPROPERTY_FLAG(...) FOR_EACH(CHECK_GPROPERTY_FLAG, SEMICOLON_END, __VA_ARGS__)

#define GPROPERTY(...) VALIDATE_EACH_GPROPERTY_FLAG(__VA_ARGS__)