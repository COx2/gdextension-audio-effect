#pragma once

#include <godot_cpp/core/class_db.hpp>

void initialize_juce_audio_plugin_host_module(godot::ModuleInitializationLevel p_level);
void uninitialize_juce_audio_plugin_host_module(godot::ModuleInitializationLevel p_level);
