cmake_minimum_required(VERSION 3.28)
project(backdoor C)

set(CMAKE_C_STANDARD 11)

add_library(backdoor STATIC library.c
        keylogger.h)
