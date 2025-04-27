#including CPM.cmake, a package manager:
#https://github.com/TheLartians/CPM.cmake
include(CPM)

set(CPM_GODOTCPP_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/external/godot-cpp")
CPMAddPackage(NAME "godotcpp" SOURCE_DIR ${CPM_GODOTCPP_SOURCE})
