file(
    GLOB_RECURSE CHEAT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(cheat SHARED ${CHEAT_SOURCES})

target_link_libraries(cheat PRIVATE Freetype::Freetype)

target_link_libraries(cheat PRIVATE NanoSVG::nanosvg NanoSVG::nanosvgrast)

target_compile_features(cheat PRIVATE cxx_std_23)

target_include_directories(cheat PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(cheat PRIVATE NOMINMAX VC_EXTRALEAN)

target_compile_options(cheat PRIVATE -Wall)
