project(helloworld)

# Automatically pick up files added to source
file(GLOB_RECURSE helloworld_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_LIST_DIR}/source/*.h"
        "${CMAKE_CURRENT_LIST_DIR}/source/*.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/source/*.cpp")

# Add a dynamic library called helloworld - this builds helloworld.dll
add_library(${PROJECT_NAME} SHARED ${helloworld_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/source")
target_link_libraries(${PROJECT_NAME} PUBLIC godot::cpp)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/source" PREFIX source FILES ${helloworld_SOURCES})
