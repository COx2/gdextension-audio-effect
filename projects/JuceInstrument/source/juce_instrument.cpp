#include "juce_instrument.h"

#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/audio_server.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace my_gdextension
{

//==============================================================================
// GDEXJuceInstrumentAudioStreamPlayback
//==============================================================================
void GDEXJuceInstrumentAudioStreamPlayback::_start(double p_from_pos)
{
}

void GDEXJuceInstrumentAudioStreamPlayback::_stop()
{
}

bool GDEXJuceInstrumentAudioStreamPlayback::_is_playing() const
{
    return true;
}

int32_t GDEXJuceInstrumentAudioStreamPlayback::_get_loop_count() const
{
    return 0;
}

double GDEXJuceInstrumentAudioStreamPlayback::_get_playback_position() const
{
    return 0.0;
}

void GDEXJuceInstrumentAudioStreamPlayback::_seek(double p_position)
{
}

int32_t GDEXJuceInstrumentAudioStreamPlayback::_mix(godot::AudioFrame* p_buffer, float p_rate_scale, int32_t p_frames)
{
    base->keyboardState;

    angleDelta = 2.0 * juce::MathConstants<double>::pi * 440.0 / currentSampleRate;

    juce::AudioBuffer<float> juce_audio_buffer(2, p_frames);

    for (int frame_idx = 0; frame_idx < p_frames; frame_idx++)
    {
        juce_audio_buffer.setSample(0, frame_idx, p_buffer[frame_idx].left);
        juce_audio_buffer.setSample(1, frame_idx, p_buffer[frame_idx].right);
    }

    float* l_ch = juce_audio_buffer.getWritePointer(0);
    float* r_ch = juce_audio_buffer.getWritePointer(1);
    for (int n = 0; n < juce_audio_buffer.getNumSamples(); n++)
    {
        l_ch[n] = r_ch[n] = std::sin(currentAngle) * currentGain;
        currentAngle += angleDelta;
    }

    for (int frame_idx = 0; frame_idx < p_frames; frame_idx++)
    {
        p_buffer[frame_idx].left = juce_audio_buffer.getSample(0, frame_idx);
        p_buffer[frame_idx].right = juce_audio_buffer.getSample(1, frame_idx);
    }

    return p_frames;
}

void GDEXJuceInstrumentAudioStreamPlayback::_tag_used_streams()
{
}

//==============================================================================
void GDEXJuceInstrumentAudioStreamPlayback::_bind_methods()
{
}

//==============================================================================
// GDEXJuceInstrumentAudioStream
//==============================================================================
GDEXJuceInstrumentAudioStream::GDEXJuceInstrumentAudioStream()
{
    keyboardState = std::make_unique<juce::MidiKeyboardState>();
}

godot::Ref<godot::AudioStreamPlayback> GDEXJuceInstrumentAudioStream::_instantiate_playback() const
{
    godot::Ref<GDEXJuceInstrumentAudioStreamPlayback> new_instance;
    new_instance.instantiate();
    new_instance->base = godot::Ref<GDEXJuceInstrumentAudioStream>(this);

    const auto mix_sample_rate = (double)godot::AudioServer::get_singleton()->get_mix_rate();
    new_instance->currentSampleRate = mix_sample_rate;

    return new_instance;
}

godot::String GDEXJuceInstrumentAudioStream::_get_stream_name() const
{
    return "UserFeed";
}

double GDEXJuceInstrumentAudioStream::_get_length() const
{
    return 0.0;
}

bool GDEXJuceInstrumentAudioStream::_is_monophonic() const
{
    return true;
}

//==============================================================================
void GDEXJuceInstrumentAudioStream::midi_note_on(int p_midi_channel, int p_midi_note_number, float p_velocity)
{
    keyboardState->noteOn(p_midi_channel, p_midi_note_number, p_velocity);
}

void GDEXJuceInstrumentAudioStream::midi_note_off(int p_midi_channel, int p_midi_note_number, float p_velocity)
{
    keyboardState->noteOff(p_midi_channel, p_midi_note_number, p_velocity);
}

void GDEXJuceInstrumentAudioStream::midi_all_notes_off(int p_midi_channel)
{
    keyboardState->allNotesOff(p_midi_channel);
}

//==============================================================================
void GDEXJuceInstrumentAudioStream::_bind_methods()
{
    godot::ClassDB::bind_method(godot::D_METHOD("midi_note_on", "midi_channel", "midi_note_number", "velocity"), &GDEXJuceInstrumentAudioStream::midi_note_on);
    godot::ClassDB::bind_method(godot::D_METHOD("midi_note_off", "midi_channel", "midi_note_number", "velocity"), &GDEXJuceInstrumentAudioStream::midi_note_off);
    godot::ClassDB::bind_method(godot::D_METHOD("midi_all_notes_off", "midi_channel"), &GDEXJuceInstrumentAudioStream::midi_all_notes_off);
}

}