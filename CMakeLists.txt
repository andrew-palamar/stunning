cmake_minimum_required(VERSION 3.22)

project(stunning VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

include(GNUInstallDirs)

add_subdirectory(src bin)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("stunningConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

install(TARGETS stunning EXPORT ${PROJECT_NAME}_Targets)

option(BUILD_COMMAND "Build the stunning command" OFF)
if (BUILD_COMMAND)
    install(TARGETS stunning-command EXPORT ${PROJECT_NAME}_Targets)
endif ()


install(EXPORT ${PROJECT_NAME}_Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

install(FILES
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/stunning/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})