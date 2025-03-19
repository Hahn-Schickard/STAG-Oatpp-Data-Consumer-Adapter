cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)

find_package(REST_DCA CONFIG REQUIRED)

add_executable(example example.cpp)
target_link_libraries(example REST_DCA::REST_DCA)
