AUX_SOURCE_DIRECTORY(. SUB_DIR_SRC_LIST)

SET(SUB_INCLUDE_DIR 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SUB_SUB_INCLUDE_DIR}
    CACHE INTERNAL "subfunc include dir" # 这个字符串相当于对变量SUB_INCLUDE_DIR的描述说明，不能省略，
                                         # 但可以自己随便定义,只有添加了这个描述SUB_INCLUDE_DIR变量才能被上层CMakeLists.txt调用！！！
)

MESSAGE(STATUS "TEST INCLUDE DIRECTORY: ${SUB_INCLUDE_DIR}")

ADD_LIBRARY()