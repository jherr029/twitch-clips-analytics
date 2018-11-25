link_directories(/usr/include)
set(CMAKE_CXX_STANDARD 11)

include_directories(../includes/)

add_library(Curl_ curl.cpp)
add_library(Json jsonParse.cpp)
add_library(Sql sqlConnector.cpp)

add_executable(lytics main.cpp)
target_link_libraries(lytics Sql Json Curl_ curl mysqlcppconn)