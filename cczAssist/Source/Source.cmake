set(CurrentDir ${CurrentDir}/Source/)
MESSAGE(STATUS "CurrentDir " ${CurrentDir})
file(GLOB_RECURSE SUB_SRC_FILES ${CurrentDir}
    ${CurrentDir}[^.]*.cpp
    ${CurrentDir}[^.]*.h
    ${CurrentDir}[^.]*.hpp
    ${CurrentDir}[^.]*.def
    ${CurrentDir}[^.]*.rc
)
list(APPEND SRC_FILES ${SUB_SRC_FILES})
