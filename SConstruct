import os
import sys

env = Environment(CPPDEFINES=[],
                LIBS=[],
                CPPPATH="./src",
                CXXFLAGS="/std:c++20",
                )

#if not using MSVC, use the GCC/Clang syntax
if not "MSVC_VERSION" in env:
    env['CXXFLAGS'] = "-std=c++20"

def AllSources(node='.', pattern='*.cpp'):
    result = []
    for dir in Glob(os.path.join(node, '*')):
        if dir.isdir():
            result += AllSources(str(dir), pattern)
    result += [
        file for file in Glob(os.path.join(node, pattern), source=True)
        if file.isfile()
    ]
    return result

sources = AllSources("./src", "*.cpp")

program = env.Program("gd-gen", sources)
Return("program")