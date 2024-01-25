if (BUILD_COMMAND)
    add_executable(stunning-command main.cpp)

    target_link_libraries(stunning-command PRIVATE stunning)

    set_target_properties(stunning-command PROPERTIES OUTPUT_NAME "stunning")
endif ()