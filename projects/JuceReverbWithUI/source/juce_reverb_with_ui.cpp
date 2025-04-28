#include "juce_reverb_with_ui.h"

#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/audio_server.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace my_gdextension
{

//==============================================================================
// GDEXJuceReverbWithUIInstance
//==============================================================================
void GDEXJuceReverbWithUIInstance::_process(const void* p_src_buffer, godot::AudioFrame* p_dst_buffer, int32_t p_frame_count)
{
    auto p_src_buffer_ = (godot::AudioFrame*)p_src_buffer;

    juce::AudioBuffer<float> juce_audio_buffer(2, p_frame_count);

    for (int frame_idx = 0; frame_idx < p_frame_count; frame_idx++)
    {
        juce_audio_buffer.setSample(0, frame_idx, p_src_buffer_[frame_idx].left);
        juce_audio_buffer.setSample(1, frame_idx, p_src_buffer_[frame_idx].right);
    }

    juce_reverb_processor->setParameters(base->editor_juce_reverb_parameters);
    juce_reverb_processor->processStereo(juce_audio_buffer.getWritePointer(0), juce_audio_buffer.getWritePointer(1), juce_audio_buffer.getNumSamples());

    for (int frame_idx = 0; frame_idx < p_frame_count; frame_idx++)
    {
        p_dst_buffer[frame_idx].left = juce_audio_buffer.getSample(0, frame_idx);
        p_dst_buffer[frame_idx].right = juce_audio_buffer.getSample(1, frame_idx);
    }
}

bool GDEXJuceReverbWithUIInstance::_process_silence() const
{
    return false;
}

//==============================================================================
void GDEXJuceReverbWithUIInstance::_bind_methods()
{
}

//==============================================================================
// GDEXJuceReverbWithUI
//==============================================================================
GDEXJuceReverbWithUI::GDEXJuceReverbWithUI()
{
    editor_juce_reverb_parameters = juce::Reverb::Parameters();
}

void GDEXJuceReverbWithUI::set_room_size(float p_value)
{
    editor_juce_reverb_parameters.roomSize = p_value;
}

float GDEXJuceReverbWithUI::get_room_size() const
{
    return editor_juce_reverb_parameters.roomSize;
}

void GDEXJuceReverbWithUI::set_damping(float p_value)
{
    editor_juce_reverb_parameters.damping = p_value;
}

float GDEXJuceReverbWithUI::get_damping() const
{
    return editor_juce_reverb_parameters.damping;
}

void GDEXJuceReverbWithUI::set_wet_level(float p_value)
{
    editor_juce_reverb_parameters.wetLevel = p_value;
}

float GDEXJuceReverbWithUI::get_wet_level() const
{
    return editor_juce_reverb_parameters.wetLevel;
}

void GDEXJuceReverbWithUI::set_dry_level(float p_value)
{
    editor_juce_reverb_parameters.dryLevel = p_value;
}

float GDEXJuceReverbWithUI::get_dry_level() const
{
    return editor_juce_reverb_parameters.dryLevel;
}

void GDEXJuceReverbWithUI::set_width(float p_value)
{
    editor_juce_reverb_parameters.width = p_value;
}

float GDEXJuceReverbWithUI::get_width() const
{
    return editor_juce_reverb_parameters.width;
}

void GDEXJuceReverbWithUI::set_freeze_mode(float p_value)
{
    editor_juce_reverb_parameters.freezeMode = p_value;
}

float GDEXJuceReverbWithUI::get_freeze_mode() const
{
    return editor_juce_reverb_parameters.freezeMode;
}

//==============================================================================
godot::Ref<godot::AudioEffectInstance> GDEXJuceReverbWithUI::_instantiate()
{
    godot::Ref<GDEXJuceReverbWithUIInstance> new_instance;
    new_instance.instantiate();
    new_instance->base = godot::Ref<GDEXJuceReverbWithUI>(this);

    const auto mix_sample_rate = (double)godot::AudioServer::get_singleton()->get_mix_rate();
    new_instance->juce_reverb_processor = std::make_unique<juce::Reverb>();
    new_instance->juce_reverb_processor->setSampleRate(mix_sample_rate);
    new_instance->processor_juce_reverb_parameters = editor_juce_reverb_parameters;

    return new_instance;
}

//==============================================================================
void GDEXJuceReverbWithUI::_bind_methods()
{
    godot::ClassDB::bind_method(godot::D_METHOD("set_room_size", "room_size"), &GDEXJuceReverbWithUI::set_room_size);
    godot::ClassDB::bind_method(godot::D_METHOD("get_room_size"), &GDEXJuceReverbWithUI::get_room_size);
    godot::ClassDB::bind_method(godot::D_METHOD("set_damping", "damping"), &GDEXJuceReverbWithUI::set_damping);
    godot::ClassDB::bind_method(godot::D_METHOD("get_damping"), &GDEXJuceReverbWithUI::get_damping);
    godot::ClassDB::bind_method(godot::D_METHOD("set_wet_level", "wet_level"), &GDEXJuceReverbWithUI::set_wet_level);
    godot::ClassDB::bind_method(godot::D_METHOD("get_wet_level"), &GDEXJuceReverbWithUI::get_wet_level);
    godot::ClassDB::bind_method(godot::D_METHOD("set_dry_level", "dry_level"), &GDEXJuceReverbWithUI::set_dry_level);
    godot::ClassDB::bind_method(godot::D_METHOD("get_dry_level"), &GDEXJuceReverbWithUI::get_dry_level);
    godot::ClassDB::bind_method(godot::D_METHOD("set_width", "width"), &GDEXJuceReverbWithUI::set_width);
    godot::ClassDB::bind_method(godot::D_METHOD("get_width"), &GDEXJuceReverbWithUI::get_width);
    godot::ClassDB::bind_method(godot::D_METHOD("set_freeze_mode", "freeze_mode"), &GDEXJuceReverbWithUI::set_freeze_mode);
    godot::ClassDB::bind_method(godot::D_METHOD("get_freeze_mode"), &GDEXJuceReverbWithUI::get_freeze_mode);

    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "room_size", godot::PROPERTY_HINT_RANGE, "0.0,1.0,0.01,suffix:"), "set_room_size", "get_room_size");
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "damping", godot::PROPERTY_HINT_RANGE, "0.0,1.0,0.01,suffix:"), "set_damping", "get_damping");
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "wet_level", godot::PROPERTY_HINT_RANGE, "0.0,1.0,0.01,suffix:"), "set_wet_level", "get_wet_level");
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "dry_level", godot::PROPERTY_HINT_RANGE, "0.0,1.0,0.01,suffix:"), "set_dry_level", "get_dry_level");
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "width", godot::PROPERTY_HINT_RANGE, "0.0,1.0,0.01,suffix:"), "set_width", "get_width");
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "freeze_mode", godot::PROPERTY_HINT_RANGE, "0.0,1.0,0.01,suffix:"), "set_freeze_mode", "get_freeze_mode");
}

}