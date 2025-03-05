function(target_link_sdl target)
    target_link_libraries(${target} PRIVATE SDL3::SDL3)
endfunction()
