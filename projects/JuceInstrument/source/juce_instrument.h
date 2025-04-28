#pragma once

#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/audio_stream_playback.hpp>
#include <godot_cpp/classes/audio_frame.hpp>

#include <juce_audio_basics/juce_audio_basics.h>
#include "JuceDemoSynthesizer.h"

namespace my_gdextension
{

//==============================================================================
class GDEXJuceInstrumentAudioStream;

//==============================================================================
class GDEXJuceInstrumentAudioStreamPlayback
    : public godot::AudioStreamPlayback
{
public:
    //==============================================================================
    virtual void _start(double p_from_pos) override;
    virtual void _stop() override;
    virtual bool _is_playing() const override;
    virtual int32_t _get_loop_count() const override;
    virtual double _get_playback_position() const override;
    virtual void _seek(double p_position) override;
    virtual int32_t _mix(godot::AudioFrame* p_buffer, float p_rate_scale, int32_t p_frames);
    virtual void _tag_used_streams() override;
    //virtual void _set_parameter(const godot::StringName& p_name, const godot::Variant& p_value);
    //virtual godot::Variant _get_parameter(const godot::StringName& p_name) const;

protected:
    //==============================================================================
    static void _bind_methods();

private:
    //==============================================================================
    GDCLASS(GDEXJuceInstrumentAudioStreamPlayback, godot::AudioStreamPlayback);
    friend class GDEXJuceInstrumentAudioStream;
    godot::Ref<GDEXJuceInstrumentAudioStream> base;

    //==============================================================================
    double currentSampleRate = 0.0;
    double currentAngle = 0.0;
    double angleDelta = 0.0;
    double currentGain = 1.0;
};

//==============================================================================
class GDEXJuceInstrumentAudioStream
    : public godot::AudioStream 
{
public:
    //==============================================================================
    GDEXJuceInstrumentAudioStream();

    //==============================================================================
    virtual godot::Ref<godot::AudioStreamPlayback> _instantiate_playback() const;
    virtual godot::String _get_stream_name() const;
    virtual double _get_length() const;
    virtual bool _is_monophonic() const;
    //virtual godot::TypedArray<godot::Dictionary> _get_parameter_list() const;

    //==============================================================================
    void midi_note_on(int p_midi_channel, int p_midi_note_number, float p_velocity);
    void midi_note_off(int p_midi_channel, int p_midi_note_number, float p_velocity);
    void midi_all_notes_off(int p_midi_channel);
    void load_sine_wave();
    void load_custom_sound(godot::String p_file_path);

protected:
    //==============================================================================
    static void _bind_methods();

private:
    //==============================================================================
    GDCLASS(GDEXJuceInstrumentAudioStream, godot::AudioStream);

    friend class GDEXJuceInstrumentAudioStreamPlayback;

    std::unique_ptr<juce::MidiKeyboardState> midiKeyboardState;
    std::unique_ptr<JuceDemoSynthesizer> demoSynthesizer;
};

}