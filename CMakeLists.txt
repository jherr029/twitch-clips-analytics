cmake_minimum_required (VERSION 3.1)

project (lytics)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")

link_directories(/usr/include)

file(GLOB SOURCES "src/*.cpp")

add_executable(lytics ${SOURCES})
target_link_libraries(lytics curl)
target_link_libraries(lytics mysqlcppconn)



