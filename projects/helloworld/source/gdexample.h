#pragma once

#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/classes/audio_effect.hpp>
#include <godot_cpp/classes/audio_effect_instance.hpp>

#include <godot_cpp/classes/audio_effect_amplify.hpp>

namespace my_gdextension
{

//==============================================================================
class AudioEffectMyAmplify;

//==============================================================================
class AudioEffectMyAmplifyInstance
    : public godot::AudioEffectInstance
{
private:
    //==============================================================================
    GDCLASS(AudioEffectMyAmplifyInstance, godot::AudioEffectInstance);
    friend class AudioEffectMyAmplify;
    godot::Ref<AudioEffectMyAmplify> base;

    float mix_volume_db;

public:
    //==============================================================================
    virtual void _process(const void* p_src_buffer, godot::AudioFrame* p_dst_buffer, int32_t p_frame_count) override;
    virtual bool _process_silence() const override;
};

//==============================================================================
class AudioEffectMyAmplify
    : public godot::AudioEffect 
{
private:
    //==============================================================================
    GDCLASS(AudioEffectMyAmplify, godot::AudioEffect);

    friend class AudioEffectMyAmplifyInstance;
    float volume_db;

protected:
    //==============================================================================
    static void _bind_methods();

public:
    //==============================================================================
    godot::Ref<godot::AudioEffectInstance> _instantiate() override;
    void set_volume_db(float p_volume);
    float get_volume_db() const;

    void set_volume_linear(float p_volume);
    float get_volume_linear() const;

    //==============================================================================
    AudioEffectMyAmplify();
};

}