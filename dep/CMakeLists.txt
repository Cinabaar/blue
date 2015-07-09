add_subdirectory(clpp)
SET(CLPP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/clpp CACHE STRING "clpp")

SET(CPPFORMAT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cppformat CACHE STRING "cppformat")
add_library(cppformat STATIC ${CPPFORMAT_INCLUDE_DIR}/format.cc)

SET(EASYLOGGINGPP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/easyloggingpp/src CACHE STRING "easyloggingpp")
SET(NOD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/nod/include CACHE STRING "signal-slot")
SET(RAPIDJSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/include CACHE STRING "rapidjson")

if(NOT GLFW_FOUND)
  add_subdirectory(glfw)
  set(GLFW_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include CACHE STRING "glfw_include_dirs")
  set(GLFW_STATIC_LIBRARIES glfw ${GLFW_STATIC_LIBRARIES} CACHE STRING "glfw_static_libraries")
endif(NOT GLFW_FOUND)

if(NOT GLEW_FOUND)
  add_subdirectory(glew)
  set(GLEW_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/glew/include CACHE STRING "glew_include_dirs")
  set(GLEW_LIBRARIES libglew_static "glew_libraries")
endif(NOT GLEW_FOUND)

if(NOT GLM_FOUND)
  SET(GLM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glm CACHE STRING "glm_include_dir")
endif(NOT GLM_FOUND)

add_subdirectory(soil)
SET(SOIL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/soil/src CACHE STRING "soil_include_dir")
