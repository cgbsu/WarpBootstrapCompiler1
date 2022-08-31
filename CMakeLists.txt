cmake_minimum_required(VERSION 2.8.12)
project(WarpBootstrapCompiler)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(MAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)

# -time -save-temps 
add_compile_options(-fconstexpr-ops-limit=3355443299 -fconstexpr-depth=1024 -fconcepts-diagnostics-depth=2 -ftemplate-depth=10000)#-g  -ggdb) #-fsanitize=leak) #-fsanitize=address)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(${CMAKE_SOURCE_DIR}/Include)
FILE(GLOB_RECURSE SourceFiles ${CMAKE_SOURCE_DIR}/Source/*.cpp)
FILE(GLOB_RECURSE TestSourceFiles ${CMAKE_SOURCE_DIR}/Test/*.cpp)
FILE(GLOB_RECURSE REPLSourceFiles ${CMAKE_SOURCE_DIR}/REPL/*.cpp)
FILE(GLOB_RECURSE CompilerSourceFiles ${CMAKE_SOURCE_DIR}/Compiler/*.cpp)
FILE(GLOB_RECURSE StackLoggerSourceFiles ${CMAKE_SOURCE_DIR}/Tools/StackLogger/*.cpp)

add_library(libwarp STATIC) 
target_sources(libwarp PUBLIC ${SourceFiles})
target_link_libraries(libwarp ${CONAN_LIBS})

add_executable(test ${TestSourceFiles})
target_sources(test PUBLIC ${TestSourceFiles})
target_link_libraries(test libwarp ${CONAN_LIBS})

#add_executable(stack_logger ${StackLoggerSourceFiles})
#target_sources(stack_logger PUBLIC ${StackLoggerSourceFiles})
#target_link_libraries(stack_logger ${CONAN_LIBS})

