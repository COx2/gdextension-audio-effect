#including CPM.cmake, a package manager:
#https://github.com/TheLartians/CPM.cmake
include(CPM)

set(CPM_GODOT_CPP_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/external/godot-cpp")
CPMAddPackage(NAME "godot-cpp" SOURCE_DIR ${CPM_GODOT_CPP_SOURCE})
