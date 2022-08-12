cmake_minimum_required(VERSION 3.21)
project(PTRenderer)

set(CMAKE_CXX_STANDARD 17)

add_library(glad STATIC thirdparty/glad/src/glad.c)
target_include_directories(glad PRIVATE thirdparty/glad/include)
add_subdirectory(thirdparty/assimp)
add_subdirectory(thirdparty/glm)
add_subdirectory(thirdparty/glfw)
#add_subdirectory(thirdparty/tinyobjloader)
aux_source_directory(core CORE_SOURCE)
aux_source_directory(common COMMON_SOURCE)

add_executable(${PROJECT_NAME} main.cpp
               ${CORE_SOURCE}
               ${COMMON_SOURCE}
                common/Type.h
        common/Utils.h core/Transform.cpp core/Transform.h)

target_include_directories(${PROJECT_NAME} PRIVATE assimp)
target_include_directories(${PROJECT_NAME} PRIVATE glm)
target_include_directories(${PROJECT_NAME} PRIVATE glfw)
target_include_directories(${PROJECT_NAME} PRIVATE thirdparty/glad/include)
target_include_directories(${PROJECT_NAME} PRIVATE common)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)
#target_link_libraries(${PROJECT_NAME} tinyloader)
