cmake_minimum_required(VERSION 3.5)

# the project
project(embedded-framework VERSION 0.1)
set(CMAKE_CXX_STANDARD 14)

# add ./cmake to CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# options
option(BUILD_TESTING "Building the tests of the driver model." OFF)
option(ENABLE_COVERAGE "Builds the code with code coverage functionality." OFF)

# for installation
include(GNUInstallDirs)

# include lib
include_directories(lib)

# load modules
include(ModuleCoverage)
include(ModuleTesting)

# library code
add_subdirectory(src/framework)
add_subdirectory(src/state)
