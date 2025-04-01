cmake_minimum_required(VERSION 3.0.2)
project(port_finder)

include_directories(
  include
)

add_library(${PROJECT_NAME} SHARED
  src/port_finder.cpp
)
