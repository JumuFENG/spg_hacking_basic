# Written to work with cmake 2.6
cmake_minimum_required (VERSION 2.6)
set (CMAKE_BACKWARDS_COMPATIBILITY 2.6)

project(SPGhbProj)

add_definitions(-DUNICODE -D_UNICODE)
include_directories(${PROJECT_SOURCE_DIR}/3rd_party/utils_win)

add_subdirectory(libMemDO)
add_subdirectory(libIPCO)
add_subdirectory(libAssist)
add_subdirectory(cczAssist)
add_subdirectory(pvztest)
