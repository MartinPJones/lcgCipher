cmake_minimum_required(VERSION 3.20)
project(lcgCipher C)

set(CMAKE_C_STANDARD 90)

add_executable(lcgCipher lcg.c cipher.c)
add_executable(lcgTest lcg.c testlcg.c)
