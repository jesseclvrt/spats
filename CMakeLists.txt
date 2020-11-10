cmake_minimum_required(VERSION 3.10.2)
project(spats)
set(CMAKE_CXX_STANDARD 17)

#json library
#find_package(nlohmann_json REQUIRED)

include_directories(include)
file(GLOB SOURCES src/*.cpp)

add_executable(spats ${SOURCES})
#target_link_libraries(spats nlohmann_json)