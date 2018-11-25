# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../includes/)

# set(CMAKE_CXX_STANDARD 11)
# set(GCC_COVERAGE_COMPILE_FLAGS "-g")
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")
link_directories(/usr/include)
set(CMAKE_CXX_STANDARD 11)

add_library(Curl_ curl.cpp)
add_library(Json jsonParse.cpp)
add_library(Sql sqlConnector.cpp)

add_executable(lytics main.cpp)
target_link_libraries(lytics curl mysqlcppconn)