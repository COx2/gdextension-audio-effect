#include "juce_audio_plugin_host.h"

#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/audio_server.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "juce_audio_plugin_editor_window.h"
#include <juce_events/juce_events.h>

namespace my_gdextension
{

//==============================================================================
// GDEXJuceAudioPluginHostAudioStreamPlayback
//==============================================================================
void GDEXJuceAudioPluginHostAudioStreamPlayback::_start(double p_from_pos)
{
}

void GDEXJuceAudioPluginHostAudioStreamPlayback::_stop()
{
}

bool GDEXJuceAudioPluginHostAudioStreamPlayback::_is_playing() const
{
    return true;
}

int32_t GDEXJuceAudioPluginHostAudioStreamPlayback::_get_loop_count() const
{
    return 0;
}

double GDEXJuceAudioPluginHostAudioStreamPlayback::_get_playback_position() const
{
    return 0.0;
}

void GDEXJuceAudioPluginHostAudioStreamPlayback::_seek(double p_position)
{
}

int32_t GDEXJuceAudioPluginHostAudioStreamPlayback::_mix(godot::AudioFrame* p_buffer, float p_rate_scale, int32_t p_frames)
{
    juce::MidiBuffer midi_messages;

    base->midiKeyboardState->processNextMidiBuffer(midi_messages, 0, p_frames, true);
    
    juce::AudioBuffer<float> juce_audio_buffer(base->monoToStereo ? 1 : 2, p_frames);
    juce_audio_buffer.clear();
    
    if (base->audioPluginInstance.get() != nullptr)
    {
        base->audioPluginInstance->processBlock(juce_audio_buffer, midi_messages);
    }

    midi_messages.clear();

    for (int frame_idx = 0; frame_idx < p_frames; frame_idx++)
    {
        p_buffer[frame_idx].left = juce_audio_buffer.getSample(0, frame_idx);
        p_buffer[frame_idx].right = juce_audio_buffer.getSample(base->monoToStereo ? 0 : 1, frame_idx);
    }

    return p_frames;
}

void GDEXJuceAudioPluginHostAudioStreamPlayback::_tag_used_streams()
{
}

//==============================================================================
void GDEXJuceAudioPluginHostAudioStreamPlayback::_bind_methods()
{
}

//==============================================================================
// GDEXJuceAudioPluginHostAudioStream
//==============================================================================
GDEXJuceAudioPluginHostAudioStream::GDEXJuceAudioPluginHostAudioStream()
    : midiKeyboardState(std::make_unique<juce::MidiKeyboardState>())
    , audioPluginInstance(nullptr)
    , audioPluginEditorWindow(nullptr)
{
    juce::initialiseJuce_GUI();

    currentSampleRate = (double)godot::AudioServer::get_singleton()->get_mix_rate();
}

godot::Ref<godot::AudioStreamPlayback> GDEXJuceAudioPluginHostAudioStream::_instantiate_playback() const
{
    godot::Ref<GDEXJuceAudioPluginHostAudioStreamPlayback> new_instance;
    new_instance.instantiate();
    new_instance->base = godot::Ref<GDEXJuceAudioPluginHostAudioStream>(this);

    const auto mix_sample_rate = (double)godot::AudioServer::get_singleton()->get_mix_rate();
    new_instance->currentSampleRate = mix_sample_rate;

    if (audioPluginInstance.get() != nullptr)
    {
        audioPluginInstance->prepareToPlay(new_instance->currentSampleRate, 512);
    }

    return new_instance;
}

godot::String GDEXJuceAudioPluginHostAudioStream::_get_stream_name() const
{
    return "UserFeed";
}

double GDEXJuceAudioPluginHostAudioStream::_get_length() const
{
    return 0.0;
}

bool GDEXJuceAudioPluginHostAudioStream::_is_monophonic() const
{
    return true;
}

//==============================================================================
void GDEXJuceAudioPluginHostAudioStream::midi_note_on(int p_midi_channel, int p_midi_note_number, float p_velocity)
{
    midiKeyboardState->noteOn(p_midi_channel, p_midi_note_number, p_velocity);
}

void GDEXJuceAudioPluginHostAudioStream::midi_note_off(int p_midi_channel, int p_midi_note_number, float p_velocity)
{
    midiKeyboardState->noteOff(p_midi_channel, p_midi_note_number, p_velocity);
}

void GDEXJuceAudioPluginHostAudioStream::midi_all_notes_off(int p_midi_channel)
{
    midiKeyboardState->allNotesOff(p_midi_channel);
}

void GDEXJuceAudioPluginHostAudioStream::load_audio_plugin(godot::String p_plugin_file_path)
{
    // Godot String to UTF-8 string to juce::String
    godot::CharString utf8_path = p_plugin_file_path.utf8();
    juce::String juce_str_path = juce::String::fromUTF8(utf8_path.get_data(), utf8_path.length());

    juce::File file_to_load = juce::File(juce_str_path);
    if (file_to_load.existsAsFile() || file_to_load.isDirectory())
    {
        bool is_new_plugin_valid = false;

        auto new_plugin_instance = VST3PluginLoader::loadVST3FromPath(file_to_load.getFullPathName());
        if (new_plugin_instance.get() != nullptr)
        {
            // Set bus layout as no input and stereo output
            juce::AudioProcessor::BusesLayout new_buses_layout;
            new_buses_layout.outputBuses.add(juce::AudioChannelSet::stereo());
            bool success = new_plugin_instance->setBusesLayout(new_buses_layout);
            if (success)
            {
                monoToStereo = false;

                new_plugin_instance->prepareToPlay(this->currentSampleRate, 512);

                if (audioPluginEditorWindow.get() != nullptr)
                {
                    audioPluginEditorWindow.reset();
                }

                audioPluginInstance.swap(new_plugin_instance);

                is_new_plugin_valid = true;
            }
            else
            {
                juce::AudioProcessor::BusesLayout new_buses_layout;
                new_buses_layout.outputBuses.add(juce::AudioChannelSet::mono());
                bool success = new_plugin_instance->setBusesLayout(new_buses_layout);
                if (success)
                {
                    monoToStereo = true;

                    new_plugin_instance->prepareToPlay(this->currentSampleRate, 512);

                    if (audioPluginEditorWindow.get() != nullptr)
                    {
                        audioPluginEditorWindow.reset();
                    }

                    audioPluginInstance.swap(new_plugin_instance);

                    is_new_plugin_valid = true;
                }
            }
        }

        if (!is_new_plugin_valid)
        {
            godot::UtilityFunctions::print(p_plugin_file_path + " is not supported");
        }
    }
}

void GDEXJuceAudioPluginHostAudioStream::open_plugin_editor()
{
    if (audioPluginInstance.get() != nullptr && audioPluginInstance->hasEditor())
    {
        if (audioPluginEditorWindow.get() != nullptr)
        {
            audioPluginEditorWindow.reset();
        }

        juce::MessageManager::callAsync([this]() {
            audioPluginEditorWindow = std::make_unique<GDEXJuceAudioPluginHostEditorWindow>(audioPluginInstance->getName());

            audioPluginEditorWindow->setUsingNativeTitleBar(true);
            audioPluginEditorWindow->setContentOwned(audioPluginInstance->createEditor(), true);
            audioPluginEditorWindow->setResizable(false, false);
            audioPluginEditorWindow->setCentreRelative(0.5f, 0.5f);
            audioPluginEditorWindow->setVisible(true);
            });
    }
}

//==============================================================================
void GDEXJuceAudioPluginHostAudioStream::_bind_methods()
{
    godot::ClassDB::bind_method(godot::D_METHOD("midi_note_on", "midi_channel", "midi_note_number", "velocity"), &GDEXJuceAudioPluginHostAudioStream::midi_note_on);
    godot::ClassDB::bind_method(godot::D_METHOD("midi_note_off", "midi_channel", "midi_note_number", "velocity"), &GDEXJuceAudioPluginHostAudioStream::midi_note_off);
    godot::ClassDB::bind_method(godot::D_METHOD("midi_all_notes_off", "midi_channel"), &GDEXJuceAudioPluginHostAudioStream::midi_all_notes_off);
    godot::ClassDB::bind_method(godot::D_METHOD("load_audio_plugin", "p_plugin_file_path"), &GDEXJuceAudioPluginHostAudioStream::load_audio_plugin);
    godot::ClassDB::bind_method(godot::D_METHOD("open_plugin_editor"), &GDEXJuceAudioPluginHostAudioStream::open_plugin_editor);
}

}