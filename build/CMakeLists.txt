cmake_minimum_required(VERSION 3.5.0)

if (WIN32)
    include(CMakePrintHelpers)

    message("Building on windows")
    # NOTE: these settings work FOR ME, more testing required
    set( CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe" )
    set( CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe" )
    include(CheckCXXSymbolExists)
endif (WIN32)

project(Vox)
enable_language(CXX)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_C_STANDARD 11)

include(CTest)
enable_testing()

# include_directories("game")
file(GLOB SOURCES
    "../src/*.cpp"
    "../src/*/*.cpp"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../")
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME})

if (UNIX)
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -lstdc++fs -fopenmp -lX11 -Wall -Ofast")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -lstdc++fs -fopenmp -lX11 -Ofast")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -lstdc++fs -fopenmp -lX11 -Wall")
endif (UNIX)
if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif (WIN32)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
