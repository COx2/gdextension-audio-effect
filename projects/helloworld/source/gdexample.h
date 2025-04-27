#pragma once

#include <godot_cpp/classes/audio_effect.hpp>
#include <godot_cpp/classes/audio_effect_instance.hpp>
#include <godot_cpp/classes/audio_frame.hpp>

namespace my_gdextension
{

//==============================================================================
class GDCustomAmplify;

//==============================================================================
class GDCustomAmplifyInstance
    : public godot::AudioEffectInstance
{
public:
    //==============================================================================
    virtual void _process(const void* src_buffer, godot::AudioFrame* dst_buffer, int32_t frame_count) override;
    virtual bool _process_silence() const override;

protected:
    //==============================================================================
    static void _bind_methods();

private:
    //==============================================================================
    GDCLASS(GDCustomAmplifyInstance, godot::AudioEffectInstance);
    friend class GDCustomAmplify;
    godot::Ref<GDCustomAmplify> base;

    float mix_volume_db;
};

//==============================================================================
class GDCustomAmplify
    : public godot::AudioEffect 
{
public:
    //==============================================================================
    GDCustomAmplify();

    //==============================================================================
    void set_volume_db(float volume);
    float get_volume_db() const;

    void set_volume_linear(float volume);
    float get_volume_linear() const;

    //==============================================================================
    godot::Ref<godot::AudioEffectInstance> _instantiate() override;

protected:
    //==============================================================================
    static void _bind_methods();

private:
    //==============================================================================
    GDCLASS(GDCustomAmplify, godot::AudioEffect);

    friend class GDCustomAmplifyInstance;
    float volume_db;
};

}