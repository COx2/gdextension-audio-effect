#include "gdexample.h"

#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/audio_server.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace my_gdextension
{

//==============================================================================
// GDCustomAmplifyInstance
//==============================================================================
void GDCustomAmplifyInstance::_process(const void* src_buffer, godot::AudioFrame* dst_buffer, int32_t frame_count)
{
    auto p_src_buffer_ = (godot::AudioFrame*)src_buffer;

    //multiply volume interpolating to avoid clicks if this changes
    float volume_db = base->volume_db;
    float vol = godot::UtilityFunctions::db_to_linear(mix_volume_db);
    float vol_inc = (godot::UtilityFunctions::db_to_linear(volume_db) - vol) / float(frame_count);

    for (int i = 0; i < frame_count; i++) 
    {
        dst_buffer[i].left = p_src_buffer_[i].left * vol;
        dst_buffer[i].right = p_src_buffer_[i].right * vol;
        vol += vol_inc;
    }
    //set volume for next mix
    mix_volume_db = volume_db;
}

bool GDCustomAmplifyInstance::_process_silence() const
{
    return false;
}

void GDCustomAmplifyInstance::_bind_methods()
{
}

//==============================================================================
// GDCustomAmplify
//==============================================================================
GDCustomAmplify::GDCustomAmplify()
{
    volume_db = 0;
}

void GDCustomAmplify::set_volume_db(float volume)
{
    volume_db = volume;
}

float GDCustomAmplify::get_volume_db() const
{
    return volume_db;
}

void GDCustomAmplify::set_volume_linear(float volume)
{
    set_volume_db(godot::UtilityFunctions::linear_to_db(volume));
}

float GDCustomAmplify::get_volume_linear() const
{
    return godot::UtilityFunctions::db_to_linear(get_volume_db());
}

godot::Ref<godot::AudioEffectInstance> GDCustomAmplify::_instantiate()
{
    godot::Ref<GDCustomAmplifyInstance> ins;
    ins.instantiate();
    ins->base = godot::Ref<GDCustomAmplify>(this);
    ins->mix_volume_db = volume_db;
    return ins;
}

void GDCustomAmplify::_bind_methods()
{
    godot::ClassDB::bind_method(godot::D_METHOD("set_volume_db", "volume"), &GDCustomAmplify::set_volume_db);
    godot::ClassDB::bind_method(godot::D_METHOD("get_volume_db"), &GDCustomAmplify::get_volume_db);
    godot::ClassDB::bind_method(godot::D_METHOD("set_volume_linear", "volume"), &GDCustomAmplify::set_volume_linear);
    godot::ClassDB::bind_method(godot::D_METHOD("get_volume_linear"), &GDCustomAmplify::get_volume_linear);

    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "volume_db", godot::PROPERTY_HINT_RANGE, "-80,24,0.01,suffix:dB"), "set_volume_db", "get_volume_db");
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "volume_linear", godot::PROPERTY_HINT_NONE, "", godot::PROPERTY_USAGE_NONE), "set_volume_linear", "get_volume_linear");
}

}