cmake_minimum_required(VERSION 3.31)
project(++C)

set(CMAKE_CXX_STANDARD 23)

add_executable(++C main.cpp
        argparse.hpp
        version.hpp
        build.hpp
        OSUtils.hpp)
