project(JuceReverb VERSION 0.1.0)

# We 'require' that we need juce. If JUCE isn't found, it will revert to what's in
# CMake/Findjuce.cmake, where you can see how JUCE is brought in/configured
find_package(juce REQUIRED)

# Automatically pick up files added to source
file(GLOB_RECURSE 
    ${PROJECT_NAME}_SOURCES 
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_LIST_DIR}/source/*.h"
    "${CMAKE_CURRENT_LIST_DIR}/source/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/source/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/*.cpp")

# Add a dynamic library called JuceReverb - this builds JuceReverb.dll
add_library(${PROJECT_NAME}
    SHARED 
    ${${PROJECT_NAME}_SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/source")

# Prepare for linking JUCE modules 
_juce_initialise_target(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        godot::cpp
        juce::juce_audio_basics
        juce::juce_audio_formats
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_dsp
        )

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/source" 
    PREFIX source 
    FILES ${${PROJECT_NAME}_SOURCES})

# Copy .gdextension to build directory.
add_custom_command(
TARGET ${PROJECT_NAME}
    POST_BUILD
        COMMAND
        ${CMAKE_COMMAND} -E
        copy "${CMAKE_CURRENT_LIST_DIR}/gdextension_module_def/${PROJECT_NAME}.gdextension" $<TARGET_FILE_DIR:${PROJECT_NAME}>
)