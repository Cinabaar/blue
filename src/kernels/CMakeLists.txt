add_custom_target(copy_kernels)
set(KERNEL_FILES sph.cl)
foreach(KERNEL_FILE ${KERNEL_FILES})
  add_custom_command(TARGET copy_kernels PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${KERNEL_FILE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/kernels/${KERNEL_FILE})
endforeach()
add_dependencies(blue copy_kernels)

add_custom_target(copy_clpp_kernels)
foreach(KERNEL_FILE ${CLPP_KERNELS})
  add_custom_command(TARGET copy_clpp_kernels PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy ${CLPP_INCLUDE_DIR}/${KERNEL_FILE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/kernels/${KERNEL_FILE})
endforeach()
add_dependencies(blue copy_clpp_kernels)

