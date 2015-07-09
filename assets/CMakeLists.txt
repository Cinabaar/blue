add_custom_target(copy_assets)
set(ASSETS skybox_texture.png)
foreach(Asset ${ASSETS})
  add_custom_command(TARGET copy_assets PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${Asset} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/${Asset})
endforeach()
add_dependencies(blue copy_assets)
