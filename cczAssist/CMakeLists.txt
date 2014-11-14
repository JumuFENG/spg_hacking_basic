cmake_minimum_required (VERSION 2.6)
set (CMAKE_BACKWARDS_COMPATIBILITY 2.6)

set(Proj_NAME cczAssist)
set(CurrentDir ${CMAKE_CURRENT_SOURCE_DIR})
include(Source/Source.cmake)
SOURCE_GROUP(${Proj_NAME} FILES ${SRC_FILES})
#MESSAGE(STATUS "JuceLibCode " ${JuceLibCode})

set(CurrentDir ${CMAKE_CURRENT_SOURCE_DIR})
include(JuceLibraryCode/JuceLibraryCode.cmake)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")
set(CMAKE_CXX_FLAGS_MINSIZEREL "/MT /O1 /Ob1 /D NDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MT /Zi /O2 /Ob1 /D NDEBUG") 
add_executable(${Proj_NAME} ${SRC_FILES} ${JuceLibCode} ${JuceModules})
