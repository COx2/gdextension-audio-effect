#pragma once

#include <godot_cpp/classes/audio_effect.hpp>
#include <godot_cpp/classes/audio_effect_instance.hpp>
#include <godot_cpp/classes/audio_frame.hpp>

namespace my_gdextension
{

//==============================================================================
class GDEXStereoAmplify;

//==============================================================================
class GDEXStereoAmplifyInstance
    : public godot::AudioEffectInstance
{
public:
    //==============================================================================
    virtual void _process(const void* p_src_buffer, godot::AudioFrame* p_dst_buffer, int32_t p_frame_count) override;
    virtual bool _process_silence() const override;

protected:
    //==============================================================================
    static void _bind_methods();

private:
    //==============================================================================
    GDCLASS(GDEXStereoAmplifyInstance, godot::AudioEffectInstance);
    friend class GDEXStereoAmplify;
    godot::Ref<GDEXStereoAmplify> base;

    float processor_volume_db_left;
    float processor_volume_db_right;
};

//==============================================================================
class GDEXStereoAmplify
    : public godot::AudioEffect 
{
public:
    //==============================================================================
    GDEXStereoAmplify();

    //==============================================================================
    void set_volume_db_left(float p_volume);
    float get_volume_db_left() const;

    void set_volume_linear_left(float p_volume);
    float get_volume_linear_left() const;

    //==============================================================================
    void set_volume_db_right(float p_volume);
    float get_volume_db_right() const;

    void set_volume_linear_right(float p_volume);
    float get_volume_linear_right() const;

    //==============================================================================
    godot::Ref<godot::AudioEffectInstance> _instantiate() override;

protected:
    //==============================================================================
    static void _bind_methods();

private:
    //==============================================================================
    GDCLASS(GDEXStereoAmplify, godot::AudioEffect);

    friend class GDEXStereoAmplifyInstance;
    float editor_volume_db_left;
    float editor_volume_db_right;
};

}