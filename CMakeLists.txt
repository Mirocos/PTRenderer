cmake_minimum_required(VERSION 3.15)
project(PTRenderer)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
add_library(glad STATIC thirdparty/glad/src/glad.c)
target_include_directories(glad PRIVATE thirdparty/glad/include)
add_subdirectory(thirdparty/assimp)
add_subdirectory(thirdparty/glm)
add_subdirectory(thirdparty/glfw)
#add_subdirectory(thirdparty/opencv)
find_package(OpenCV REQUIRED)
#add_subdirectory(thirdparty/tinyobjloader)
aux_source_directory(core CORE_SOURCE)
aux_source_directory(common COMMON_SOURCE)

add_executable(${PROJECT_NAME} main.cpp
               ${CORE_SOURCE}
               ${COMMON_SOURCE}
                common/Type.h
        common/Utils.h core/Transform.cpp core/Transform.h
        thirdparty/pugixml/src/pugixml.cpp
        )

target_include_directories(${PROJECT_NAME} PRIVATE assimp)
target_include_directories(${PROJECT_NAME} PRIVATE glm)
target_include_directories(${PROJECT_NAME} PRIVATE glfw)
target_include_directories(${PROJECT_NAME} PRIVATE thirdparty/glad/include)
target_include_directories(${PROJECT_NAME} PRIVATE common)
target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE assimp)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenMP_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
message(STATUS ${OpenCV_LIBS})
if(UNIX)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_DL_LIBS})
endif(UNIX)
#target_link_libraries(${PROJECT_NAME} tinyloader)
