#include "juce_audio_plugin_editor_window.h"

namespace my_gdextension
{

GDEXJuceAudioPluginHostEditorWindow::GDEXJuceAudioPluginHostEditorWindow(const juce::String& pluginName)
    : juce::DocumentWindow(pluginName,
        juce::Colours::darkcyan,
        juce::DocumentWindow::TitleBarButtons::minimiseButton | juce::DocumentWindow::TitleBarButtons::closeButton,
        true)
{
}

GDEXJuceAudioPluginHostEditorWindow::~GDEXJuceAudioPluginHostEditorWindow()
{
}

void GDEXJuceAudioPluginHostEditorWindow::closeButtonPressed()
{
    setVisible(false);
}

}
