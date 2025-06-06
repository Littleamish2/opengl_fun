cmake_minimum_required(VERSION 3.13)


add_subdirectory("thirdparty/glfw-3.4")
add_subdirectory("thirdparty/glad")


project(main1)
set(CMAKE_CXX_STANDARD 17)

# Mac-specific: OpenGL is exposed as a system framework
find_package(OpenGL REQUIRED)

# we define MY_SOURCES to be a list of all the source files for my project
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(main1 "${MY_SOURCES}")


target_include_directories(main1 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(main1 PRIVATE glfw OpenGL::GL glad)
