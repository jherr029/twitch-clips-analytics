link_directories(/usr/include)
set(CMAKE_CXX_STANDARD 11)


include_directories(../includes/)
include_directories(src)

add_library(Curl_ curl.cpp ../includes/curl.h)
add_library(Json jsonParse.cpp ../includes/jsonParse.h)
add_library(Sql sqlConnector.cpp ../includes/sqlConnector.h)
add_library(errorFlag errorFlag.cpp ../includes/errorFlag.h)
add_library(lyticsGather lyticsGather.cpp ../includes/lyticsGather.h)
add_library(analyzer analyzer.cpp ../includes/analyzer.h)

# add_executable(lytics main.cpp curl.cpp errorFlag.cpp jsonParse.cpp sqlConnector.cpp )
add_executable(lytics main.cpp lyticsGather.cpp curl.cpp analyzer.cpp)
# add_executable(curlcpp curl.cpp)
target_link_libraries(lytics Sql Json Curl_ lyticsGather errorFlag analyzer curl mysqlcppconn)
# target_link_libraries(curlcpp Json curl Curl_)