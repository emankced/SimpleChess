cmake_minimum_required(VERSION 3.0)

project(simplechess)

set(CMAKE_CXX_STANDARD 17)

add_executable(simplechess main.cpp Chess.cpp Pieces.cpp common.cpp Board.cpp)

install(TARGETS simplechess RUNTIME DESTINATION bin)
