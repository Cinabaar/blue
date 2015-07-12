cmake_minimum_required(VERSION 2.8.4)
project(blue)

set(CMAKE_CXX_FLAGS -std=c++11)
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

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

if(NOT GLFW_FOUND)
  add_subdirectory(dep/glfw)
  set(GLFW_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/dep/glfw/include CACHE STRING "glfw_include_dirs" FORCE)
  set(GLFW_STATIC_LIBRARIES glfw ${GLFW_STATIC_LIBRARIES} CACHE STRING "glfw_static_libraries" FORCE)
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/dep)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/assets)
