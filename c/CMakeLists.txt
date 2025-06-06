cmake_minimum_required(VERSION 3.10)
project(digitalocean-c VERSION 1.0.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Build options
option(BUILD_SHARED_LIBS "Build shared library" ON)
option(BUILD_CLI "Build CLI application" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTS "Build tests" OFF)

# Find required packages
find_package(PkgConfig REQUIRED)
find_package(CURL REQUIRED)

# Find cJSON
pkg_check_modules(CJSON REQUIRED libcjson)

# Include directories
include_directories(include)
include_directories(${CURL_INCLUDE_DIRS})
include_directories(${CJSON_INCLUDE_DIRS})

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

# Library source files
set(LIB_SOURCES
    src/client.c
    src/config.c
    src/http.c
    src/json.c
)

# Create library
add_library(digitalocean ${LIB_SOURCES})
target_link_libraries(digitalocean ${CURL_LIBRARIES} ${CJSON_LIBRARIES})
target_compile_options(digitalocean PRIVATE ${CJSON_CFLAGS_OTHER})

# Set library properties
set_target_properties(digitalocean PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER "include/digitalocean/client.h;include/digitalocean/config.h;include/digitalocean/types.h;include/digitalocean/http.h"
)

# CLI application
if(BUILD_CLI)
    set(CLI_SOURCES
        src/cli/main.c
        src/cli/account.c
        src/cli/droplets.c
        src/cli/config.c
    )
    
    add_executable(do-cli ${CLI_SOURCES})
    target_link_libraries(do-cli digitalocean)
endif()

# Examples
if(BUILD_EXAMPLES)
    add_executable(example_basic examples/basic_usage.c)
    target_link_libraries(example_basic digitalocean)
endif()

# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Install targets
install(TARGETS digitalocean
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/digitalocean
)

if(BUILD_CLI)
    install(TARGETS do-cli DESTINATION bin)
endif()

# Pkg-config file
configure_file(digitalocean.pc.in digitalocean.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/digitalocean.pc DESTINATION lib/pkgconfig)