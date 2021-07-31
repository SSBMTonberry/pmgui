cmake_minimum_required(VERSION 3.12)
project(pmgui_project)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 4)
set(PROJECT_VERSION_PATCH 1)
set(CMAKE_CXX_FLAGS -DSFML_STATIC)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSFML_STATIC -DPMGUI_DEBUG")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
            "Choose the type of build, options are: Debug Release
RelWithDebInfo MinSizeRel."
            FORCE)
endif(NOT CMAKE_BUILD_TYPE)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DEBUG 1)
    message(DEBUGGIN')
else()
    set(RELEASE 1)
endif()

if(UNIX AND NOT APPLE)
    set(LINUX 1)
    set(WINDOWS 0)
    set(APPLE 0)
    set(MSVC 0)
    set(MINGW 0)
elseif(APPLE)
    link_directories(/usr/local/opt/llvm/lib/)
    set(LINUX 0)
    set(WINDOWS 0)
    set(APPLE 1)
    set(MSVC 0)
    set(MINGW 0)
elseif(WIN32)
    set(LINUX 0)
    set(WINDOWS 1)
    set(APPLE 0)
    if(MSVC)
        set(MSVC 1)
        set(MINGW 0)
    elseif(MINGW)
        set(MINGW 1)
        set(MSVC 0)
    endif()
else()
    set(LINUX 0)
    set(WINDOWS 0)
    set(APPLE 0)
    set(MSVC 0)
    set(MINGW 0)
endif()

option(BUILD_SHARED_LIBS "Build shared libraries." OFF)
option(BUILD_TESTS "Build unit tests." ON)
option(BUILD_DEMO "Build demos." ON)

#configure_file (
#        "${PROJECT_SOURCE_DIR}/PmguiConfig.h.in"
#        "${PROJECT_SOURCE_DIR}/src/PmguiConfig.h")

add_subdirectory(external_libs/sfml)
add_subdirectory(src)
if(${BUILD_DEMO})
    add_subdirectory(demo)
endif()
if(${BUILD_TESTS})
    add_subdirectory(tests)
endif()

unset(BUILD_SHARED_LIBS CACHE) #Must clear the cache to detect changes in this variable
unset(BUILD_TESTS CACHE) #Must clear the cache to detect changes in this variable
unset(BUILD_DEMO CACHE)

#set(SOURCE_FILES main.cpp)
#add_executable(Pixelmound ${SOURCE_FILES})