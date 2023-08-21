PROJECT(MODEL_TSET)

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    MESSAGE(STATUS "Configure on/for Linux")
    INCLUDE_DIRECTORIES(../../include /usr/include/eigen3)
ELSEIF (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    MESSAGE(STATUS "Configure on/for Windos")
    INCLUDE_DIRECTORIES(../../include)
ENDIF()

FILE(GLOB_RECURSE SRC_DIR_LIST "../../src/*.cpp" "../../src/*.c" "*.cpp")	#遍历获取source/路径下所有的*.cpp和*.c文件列表

ADD_EXECUTABLE(${PROJECT_NAME} ${SRC_DIR_LIST})