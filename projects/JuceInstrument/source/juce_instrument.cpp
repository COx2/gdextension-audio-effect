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
    juce::MidiBuffer midi_messages;

    base->midiKeyboardState->processNextMidiBuffer(midi_messages, 0, p_frames, true);
    
    juce::AudioBuffer<float> juce_audio_buffer(2, p_frames);
    juce_audio_buffer.clear();
    
    base->demoSynthesizer->renderNextBlock(juce_audio_buffer, midi_messages, 0, juce_audio_buffer.getNumSamples());

    midi_messages.clear();

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
    midiKeyboardState = std::make_unique<juce::MidiKeyboardState>();
    demoSynthesizer = std::make_unique<JuceDemoSynthesizer>();
    demoSynthesizer->loadSineWave();
}

godot::Ref<godot::AudioStreamPlayback> GDEXJuceInstrumentAudioStream::_instantiate_playback() const
{
    godot::Ref<GDEXJuceInstrumentAudioStreamPlayback> new_instance;
    new_instance.instantiate();
    new_instance->base = godot::Ref<GDEXJuceInstrumentAudioStream>(this);

    const auto mix_sample_rate = (double)godot::AudioServer::get_singleton()->get_mix_rate();
    new_instance->currentSampleRate = mix_sample_rate;

    demoSynthesizer->setCurrentPlaybackSampleRate(new_instance->currentSampleRate);

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
    midiKeyboardState->noteOn(p_midi_channel, p_midi_note_number, p_velocity);
}

void GDEXJuceInstrumentAudioStream::midi_note_off(int p_midi_channel, int p_midi_note_number, float p_velocity)
{
    midiKeyboardState->noteOff(p_midi_channel, p_midi_note_number, p_velocity);
}

void GDEXJuceInstrumentAudioStream::midi_all_notes_off(int p_midi_channel)
{
    midiKeyboardState->allNotesOff(p_midi_channel);
}

void GDEXJuceInstrumentAudioStream::load_sine_wave()
{
    demoSynthesizer->loadSineWave();
}

void GDEXJuceInstrumentAudioStream::load_custom_sound(godot::String p_file_path)
{
    // Godot String to UTF-8 string to juce::String
    godot::CharString utf8_path = p_file_path.utf8();
    juce::String juce_str_path = juce::String::fromUTF8(utf8_path.get_data(), utf8_path.length());

    juce::File file_to_load = juce::File(juce_str_path);
    if (file_to_load.existsAsFile())
    {
        demoSynthesizer->loadAudioSample(file_to_load.createInputStream());
    }
}

//==============================================================================
void GDEXJuceInstrumentAudioStream::_bind_methods()
{
    godot::ClassDB::bind_method(godot::D_METHOD("midi_note_on", "midi_channel", "midi_note_number", "velocity"), &GDEXJuceInstrumentAudioStream::midi_note_on);
    godot::ClassDB::bind_method(godot::D_METHOD("midi_note_off", "midi_channel", "midi_note_number", "velocity"), &GDEXJuceInstrumentAudioStream::midi_note_off);
    godot::ClassDB::bind_method(godot::D_METHOD("midi_all_notes_off", "midi_channel"), &GDEXJuceInstrumentAudioStream::midi_all_notes_off);
    godot::ClassDB::bind_method(godot::D_METHOD("load_sine_wave"), &GDEXJuceInstrumentAudioStream::load_sine_wave);
    godot::ClassDB::bind_method(godot::D_METHOD("load_custom_sound", "file_path"), &GDEXJuceInstrumentAudioStream::load_custom_sound);
}

}