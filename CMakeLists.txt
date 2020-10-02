cmake_minimum_required(VERSION 2.8)

PROJECT(PYVM01)

# SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g -ggdb -rdynamic")
# SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -rdynamic")

SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
# SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g -ggdb -rdynamic")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -rdynamic")

ADD_EXECUTABLE(pyvm01 main.cpp
    object/hiInteger.cpp
    object/hiString.cpp
    util/arrayList.cpp
    runtime/interpreter.cpp
    code/binaryFileParser.cpp
    code/codeObject.cpp)

    
INCLUDE_DIRECTORIES(./)

