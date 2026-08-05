cmake_minimum_required(VERSION 3.14)
project(HostelCMS VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include Directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source Files
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Executable Target
add_executable(HostelCMSBackend ${SOURCES})

# Platform Specific Linkings & Thread Support
find_package(Threads REQUIRED)
target_link_libraries(HostelCMSBackend PRIVATE Threads::Threads)

if(WIN32)
    target_link_libraries(HostelCMSBackend PRIVATE ws2_32)
endif()

# Optional MySQL Connector/C++ Support
find_library(MYSQL_LIB NAMES mysqlclient mysqlcppconn mysql)
if(MYSQL_LIB)
    message(STATUS "MySQL library found: ${MYSQL_LIB}")
    target_link_libraries(HostelCMSBackend PRIVATE ${MYSQL_LIB})
else()
    message(STATUS "MySQL Connector library not linked directly (Embedded fallback engine enabled).")
endif()

# Compiler Warning Flags
if(MSVC)
    target_compile_options(HostelCMSBackend PRIVATE /W4 /EHsc /wd4996)
else()
    target_compile_options(HostelCMSBackend PRIVATE -Wall -Wextra -Wpedantic)
endif()
