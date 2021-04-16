project(portability)

FILE(GLOB portability_SRCS *.cpp)
FILE(GLOB portability_HDRS *.hpp)

add_library(portability ${portability_HDRS} ${portability_SRCS})
