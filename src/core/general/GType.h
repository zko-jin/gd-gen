#pragma once

enum class GType
{
    Invalid = 0,
    Float = 1,
    Int = 2,
    Node = 3,
    Resource = 4,
    NodePathToRaw = 5,
    Object = 6,
    Boolean = 7,
    PackedByteArray = 8,
    PackedInt32Array,
    PackedInt64Array,
    PackedFloat32Array,
    PackedFloat64Array,
    PackedStringArray,
    PackedVector2Array,
    PackedVector3Array,
    PackedColorArray,
    PackedVector4Array,
    String,
    Variant,
    Enum
};