#include "stereo_amplify.h"

#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/audio_server.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace my_gdextension
{

//==============================================================================
// GDEXStereoAmplifyInstance
//==============================================================================
void GDEXStereoAmplifyInstance::_process(const void* p_src_buffer, godot::AudioFrame* p_dst_buffer, int32_t p_frame_count)
{
    auto p_src_buffer_ = (godot::AudioFrame*)p_src_buffer;

    //multiply volume interpolating to avoid clicks if this changes
    float editor_volume_db_left = base->editor_volume_db_left;
    float vol_cur_left = godot::UtilityFunctions::db_to_linear(processor_volume_db_left);
    float vol_inc_left = (godot::UtilityFunctions::db_to_linear(editor_volume_db_left) - vol_cur_left) / float(p_frame_count);

    float editor_volume_db_right = base->editor_volume_db_right;
    float vol_cur_right = godot::UtilityFunctions::db_to_linear(processor_volume_db_right);
    float vol_inc_right = (godot::UtilityFunctions::db_to_linear(editor_volume_db_right) - vol_cur_right) / float(p_frame_count);

    for (int i = 0; i < p_frame_count; i++) 
    {
        p_dst_buffer[i].left = p_src_buffer_[i].left * vol_cur_left;
        vol_cur_left += vol_inc_left;

        p_dst_buffer[i].right = p_src_buffer_[i].right * vol_cur_right;
        vol_cur_right += vol_inc_right;
    }

    //set volume for next mix
    processor_volume_db_left = editor_volume_db_left;
    processor_volume_db_right = editor_volume_db_right;
}

bool GDEXStereoAmplifyInstance::_process_silence() const
{
    return false;
}

//==============================================================================
void GDEXStereoAmplifyInstance::_bind_methods()
{
}

//==============================================================================
// GDEXStereoAmplify
//==============================================================================
GDEXStereoAmplify::GDEXStereoAmplify()
{
    editor_volume_db_left = 0.0f;
    editor_volume_db_right = 0.0f;
}

//==============================================================================
void GDEXStereoAmplify::set_volume_db_left(float p_volume_left)
{
    editor_volume_db_left = p_volume_left;
}

float GDEXStereoAmplify::get_volume_db_left() const
{
    return editor_volume_db_left;
}

void GDEXStereoAmplify::set_volume_linear_left(float p_volume)
{
    set_volume_db_left(godot::UtilityFunctions::linear_to_db(p_volume));
}

float GDEXStereoAmplify::get_volume_linear_left() const
{
    return godot::UtilityFunctions::db_to_linear(get_volume_db_left());
}

void GDEXStereoAmplify::set_volume_db_right(float p_volume)
{
    editor_volume_db_right = p_volume;
}

float GDEXStereoAmplify::get_volume_db_right() const
{
    return editor_volume_db_right;
}

void GDEXStereoAmplify::set_volume_linear_right(float p_volume)
{
    set_volume_db_right(godot::UtilityFunctions::linear_to_db(p_volume));
}

float GDEXStereoAmplify::get_volume_linear_right() const
{
    return godot::UtilityFunctions::db_to_linear(get_volume_db_right());
}

//==============================================================================
godot::Ref<godot::AudioEffectInstance> GDEXStereoAmplify::_instantiate()
{
    godot::Ref<GDEXStereoAmplifyInstance> new_instance;
    new_instance.instantiate();
    new_instance->base = godot::Ref<GDEXStereoAmplify>(this);
    new_instance->processor_volume_db_left = editor_volume_db_left;
    new_instance->processor_volume_db_right = editor_volume_db_right;

    return new_instance;
}

//==============================================================================
void GDEXStereoAmplify::_bind_methods()
{
    godot::ClassDB::bind_method(godot::D_METHOD("set_volume_db_left", "volume_left"), &GDEXStereoAmplify::set_volume_db_left);
    godot::ClassDB::bind_method(godot::D_METHOD("get_volume_db_left"), &GDEXStereoAmplify::get_volume_db_left);
    godot::ClassDB::bind_method(godot::D_METHOD("set_volume_linear_left", "volume_left"), &GDEXStereoAmplify::set_volume_linear_left);
    godot::ClassDB::bind_method(godot::D_METHOD("get_volume_linear_left"), &GDEXStereoAmplify::get_volume_linear_left);

    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "volume_db_left", godot::PROPERTY_HINT_RANGE, "-80,24,0.01,suffix:dB"), "set_volume_db_left", "get_volume_db_left");
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "volume_linear_left", godot::PROPERTY_HINT_NONE, "", godot::PROPERTY_USAGE_NONE), "set_volume_linear_left", "get_volume_linear_left");

    godot::ClassDB::bind_method(godot::D_METHOD("set_volume_db_right", "volume_right"), &GDEXStereoAmplify::set_volume_db_right);
    godot::ClassDB::bind_method(godot::D_METHOD("get_volume_db_right"), &GDEXStereoAmplify::get_volume_db_right);
    godot::ClassDB::bind_method(godot::D_METHOD("set_volume_linear_right", "volume_right"), &GDEXStereoAmplify::set_volume_linear_right);
    godot::ClassDB::bind_method(godot::D_METHOD("get_volume_linear_right"), &GDEXStereoAmplify::get_volume_linear_right);

    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "volume_db_right", godot::PROPERTY_HINT_RANGE, "-80,24,0.01,suffix:dB"), "set_volume_db_right", "get_volume_db_right");
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "volume_linear_right", godot::PROPERTY_HINT_NONE, "", godot::PROPERTY_USAGE_NONE), "set_volume_linear_right", "get_volume_linear_right");
}

}