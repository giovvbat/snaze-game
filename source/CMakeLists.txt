cmake_minimum_required(VERSION 3.10)
project(Snaze)
set (CMAKE_CXX_STANDARD 11)

add_executable(snaze
               main.cpp
               Snake.cpp
               SnakeGame.cpp
               SnakeLevel.cpp
               RSPlayer.cpp
               BTSPlayer.cpp)
