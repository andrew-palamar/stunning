option(BUILD_TESTS "Build tests" OFF)
if (BUILD_TESTS)

    include(FetchContent)

    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.5.2)

    FetchContent_MakeAvailable(Catch2)

    add_executable(tests main.cpp)

    target_link_libraries(tests PRIVATE stunning)
    target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

    set_target_properties(tests PROPERTIES OUTPUT_NAME "stunning-tests")

    install(TARGETS tests)
endif ()