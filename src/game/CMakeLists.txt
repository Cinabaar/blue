set(SOURCE_FILES ${SOURCE_FILES}
  ${CMAKE_CURRENT_SOURCE_DIR}/game.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/scene.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/scene_builder.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/entity.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/components/model_component.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/components/render_component.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/components/transform_component.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/components/camera_component.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/components/camera_logic_component.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/components/skybox_render_component.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/components/water_render_component.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/components/water_model_component.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/components/water_logic_component.cpp

  PARENT_SCOPE)
