PROJECT(TEST)

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    MESSAGE(STATUS "Configure on/for Linux")
    INCLUDE_DIRECTORIES(/usr/local/include/eigen3 /usr/include/eigen3)
ELSEIF (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    MESSAGE(STATUS "Configure on/for Windos")
ENDIF()

ADD_SUBDIRECTORY(geometry_test)
ADD_SUBDIRECTORY(model_test)
ADD_SUBDIRECTORY(solver_test)
ADD_SUBDIRECTORY(complete_test)


