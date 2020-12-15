project(mmtk_dotnet)

cmake_minimum_required(VERSION 3.14.5 FATAL_ERROR)
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-as-needed")
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)

# MMTk
set(MMTK_LIBRARY_PATH "../mmtk/target/debug")
get_filename_component(ABSOLUTE_MMTK_LIBRARY_PATH ${MMTK_LIBRARY_PATH} ABSOLUTE)
set(MMTK_HEADER "mmtk")
get_filename_component(ABSOLUTE_MMTK_HEADER ${MMTK_HEADER} ABSOLUTE)

# CoreCLR GC path
set(CORECLR_FOR_CUSTOMGC "../repos/dotnet/src/coreclr")
get_filename_component(ABSOLUTE_CORECLR_FOR_CUSTOM_GC ${CORECLR_FOR_CUSTOMGC} ABSOLUTE)

# Defaut build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Handle build type
if (CMAKE_BUILD_TYPE MATCHES [Dd]ebug)
    message(STATUS "Debug build")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
else()
    message(STATUS "Release build")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

# Add subdirectories
add_subdirectory(src)

