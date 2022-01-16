cmake_minimum_required(VERSION 3.21)
project(PTRenderer)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(thirdparty/assimp)
add_subdirectory(thirdparty/glm)
aux_source_directory(core CORE_SOURCE)
aux_source_directory(common COMMON_SOURCE)

add_executable(${PROJECT_NAME} main.cpp
               ${CORE_SOURCE}
               ${COMMON_SOURCE}
        )


target_include_directories(${PROJECT_NAME} PRIVATE assimp)
target_include_directories(${PROJECT_NAME} PRIVATE glm)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)


