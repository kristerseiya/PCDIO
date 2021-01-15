cmake_minimum_required(VERSION 3.17)
project(pcdio)

set(CMAKE_CXX_STANDARD 11)

# OPTION
############################
option(BUILD_PCDIO_PYTHON "build pointcloud io python interface" ON)
############################


# EDIT PATHS ACCORDINGLY
############################
set(PCDIO_PYTHON_INSTALL_PATH
          /Users/Krister/anaconda3/envs/python38/lib/python3.8/site-packages)

find_package(pybind11 REQUIRED HINTS /usr/local/opt/pybind11)
############################



file(GLOB PCDIO_SOURCES
        source/readply.cpp
        source/readstl.cpp
        source/readxyzm.cpp
        source/readxxx.cpp)

add_library(pcdio STATIC ${PCDIO_SOURCES})
target_include_directories(pcdio PRIVATE include)

pybind11_add_module(pypcdio pybind/pypcdio.cpp ${PCDIO_SOURCES})
target_include_directories(pypcdio PRIVATE include)
install(TARGETS pypcdio DESTINATION ${PCDIO_PYTHON_INSTALL_PATH})
