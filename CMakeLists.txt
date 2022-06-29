cmake_minimum_required(VERSION 2.8.12)
project(WarpBootstrapCompiler)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

add_compile_options(-fconstexpr-ops-limit=3355443299)#-g  -ggdb) #-fsanitize=leak) #-fsanitize=address)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(${CMAKE_SOURCE_DIR}/Include)
FILE(GLOB SourceFiles ${CMAKE_SOURCE_DIR}/Source/*.cpp)
FILE(GLOB TestSourceFiles ${CMAKE_SOURCE_DIR}/Test/*.cpp)
FILE(GLOB REPLSourceFiles ${CMAKE_SOURCE_DIR}/REPL/*.cpp)
FILE(GLOB CompilerSourceFiles ${CMAKE_SOURCE_DIR}/Compiler/*.cpp)
add_library(libwarp STATIC) 
target_sources(libwarp PUBLIC ${SourceFiles})
target_link_libraries(libwarp ${CONAN_LIBS})
add_executable(test ${TestSourceFiles})
target_link_libraries(test libwarp ${CONAN_LIBS})

