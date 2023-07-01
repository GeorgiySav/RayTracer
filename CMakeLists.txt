cmake_minimum_required(VERSION 3.21)
project(RayTracer LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

include_directories(include/)
add_subdirectory(include/glm)

find_package(glbinding REQUIRED)

file(GLOB SRCFILES CONFIGURE_DEPENDS "src/*.h" "src/*.cpp" "src/rendering/*.h" "src/rendering/*.cpp")

add_executable(${CMAKE_PROJECT_NAME} ${SRCFILES})

target_link_libraries(
    ${CMAKE_PROJECT_NAME} 
    PUBLIC 
    glm
    glbinding::glbinding
    glbinding::glbinding-aux 
    PRIVATE 
    sfml-graphics
    sfml-window)

target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_17)

if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${CMAKE_PROJECT_NAME}> $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS ${CMAKE_PROJECT_NAME})
