#include "gdexample.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace my_gdextension
{

//==============================================================================
void AudioEffectMyAmplifyInstance::_process(const void* p_src_buffer, godot::AudioFrame* p_dst_buffer, int32_t p_frame_count)
{
    return;

    auto p_src_buffer_ = (godot::AudioFrame*)p_src_buffer;

    //multiply volume interpolating to avoid clicks if this changes
    float volume_db = base->volume_db;
    float vol = godot::UtilityFunctions::db_to_linear(mix_volume_db);
    float vol_inc = (godot::UtilityFunctions::db_to_linear(volume_db) - vol) / float(p_frame_count);

    for (int i = 0; i < p_frame_count; i++) 
    {
        p_dst_buffer[i].left = p_src_buffer_[i].left * vol;
        p_dst_buffer[i].right = p_src_buffer_[i].right * vol;
        vol += vol_inc;
    }
    //set volume for next mix
    mix_volume_db = volume_db;
}

bool AudioEffectMyAmplifyInstance::_process_silence() const
{
    return false;
}

//==============================================================================
godot::Ref<godot::AudioEffectInstance> AudioEffectMyAmplify::_instantiate()
{
    godot::Ref<AudioEffectMyAmplifyInstance> ins;
    ins.instantiate();
    ins->base = godot::Ref<AudioEffectMyAmplifyInstance>(this);
    ins->mix_volume_db = volume_db;
    return ins;
}

void AudioEffectMyAmplify::set_volume_db(float p_volume)
{
    volume_db = p_volume;
}

float AudioEffectMyAmplify::get_volume_db() const
{
    return volume_db;
}

void AudioEffectMyAmplify::set_volume_linear(float p_volume)
{
    set_volume_db(godot::UtilityFunctions::linear_to_db(p_volume));
}

float AudioEffectMyAmplify::get_volume_linear() const
{
    return godot::UtilityFunctions::db_to_linear(get_volume_db());
}

void AudioEffectMyAmplify::_bind_methods()
{
    godot::ClassDB::bind_method(godot::D_METHOD("set_volume_db", "volume"), &AudioEffectMyAmplify::set_volume_db);
    godot::ClassDB::bind_method(godot::D_METHOD("get_volume_db"), &AudioEffectMyAmplify::get_volume_db);
    godot::ClassDB::bind_method(godot::D_METHOD("set_volume_linear", "volume"), &AudioEffectMyAmplify::set_volume_linear);
    godot::ClassDB::bind_method(godot::D_METHOD("get_volume_linear"), &AudioEffectMyAmplify::get_volume_linear);

    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "volume_db", godot::PROPERTY_HINT_RANGE, "-80,24,0.01,suffix:dB"), "set_volume_db", "get_volume_db");
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "volume_linear", godot::PROPERTY_HINT_NONE, "", godot::PROPERTY_USAGE_NONE), "set_volume_linear", "get_volume_linear");
}

AudioEffectMyAmplify::AudioEffectMyAmplify()
{
    volume_db = 0;
}



}