cmake_minimum_required(VERSION 2.8.4)
project(blue)

set(CMAKE_CXX_FLAGS -std=c++11)
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include(cotire)

find_package(OpenCL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PkgConfig)

if(PKG_CONFIG_FOUND)
  MESSAGE("Package config found")
  pkg_search_module(GLFW glfw3)
  if(GLFW_FOUND)
    MESSAGE("System GLFW found")
  endif()
endif()

find_package(GLEW)
if(GLEW_FOUND)
  MESSAGE("System GLEW found")
endif()

find_package(GLM)
if(GLM_FOUND)
  MESSAGE("System GLM found")
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/dep)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/assets)
