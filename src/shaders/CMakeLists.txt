add_custom_target(copy_shaders)
set(SHADER_FILES shader.frag shader.vert watershader.frag watershader.vert watershader.geom skybox.vert skybox.frag)
foreach(ShaderFile ${SHADER_FILES})
  add_custom_command(TARGET copy_shaders PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${ShaderFile} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/${ShaderFile})
endforeach()
add_dependencies(blue copy_shaders)
