add_library(stunning INTERFACE include/stunning.h)

target_include_directories(stunning INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

add_library(stunning::stunning ALIAS stunning)