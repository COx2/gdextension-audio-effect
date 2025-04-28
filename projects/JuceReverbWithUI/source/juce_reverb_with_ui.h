#pragma once

#include <godot_cpp/classes/audio_effect.hpp>
#include <godot_cpp/classes/audio_effect_instance.hpp>
#include <godot_cpp/classes/audio_frame.hpp>

#include <juce_audio_basics/juce_audio_basics.h>

namespace my_gdextension
{

//==============================================================================
class GDEXJuceReverbWithUI;

//==============================================================================
class GDEXJuceReverbWithUIInstance
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
    GDCLASS(GDEXJuceReverbWithUIInstance, godot::AudioEffectInstance);
    friend class GDEXJuceReverbWithUI;
    godot::Ref<GDEXJuceReverbWithUI> base;

    std::unique_ptr<juce::Reverb> juce_reverb_processor;
    juce::Reverb::Parameters processor_juce_reverb_parameters;
};

//==============================================================================
class GDEXJuceReverbWithUI
    : public godot::AudioEffect 
{
public:
    //==============================================================================
    GDEXJuceReverbWithUI();

    //==============================================================================
    void set_room_size(float p_value);
    float get_room_size() const;

    void set_damping(float p_value);
    float get_damping() const;

    void set_wet_level(float p_value);
    float get_wet_level() const;

    void set_dry_level(float p_value);
    float get_dry_level() const;

    void set_width(float p_value);
    float get_width() const;

    void set_freeze_mode(float p_value);
    float get_freeze_mode() const;

    //==============================================================================
    godot::Ref<godot::AudioEffectInstance> _instantiate() override;

protected:
    //==============================================================================
    static void _bind_methods();

private:
    //==============================================================================
    GDCLASS(GDEXJuceReverbWithUI, godot::AudioEffect);

    friend class GDEXJuceReverbWithUIInstance;
    juce::Reverb::Parameters editor_juce_reverb_parameters;
};

}