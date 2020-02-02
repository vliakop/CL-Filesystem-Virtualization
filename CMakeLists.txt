cmake_minimum_required(VERSION 3.13)
project(CFS__delis4_2019_)

set(CMAKE_CXX_STANDARD 98)

add_executable(CFS__delis4_2019_ main.cpp Functions/Commands.h Functions/Commands.cpp Structures/Datastream.h Structures/Datastream.cpp Structures/Metadata.h Structures/Metadata.cpp Structures/Superblock.h Structures/Superblock.cpp Structures/DirData.h Structures/DirData.cpp Functions/MyFunctions.hpp Functions/MyFunctions.cpp)