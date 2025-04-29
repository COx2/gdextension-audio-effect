#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace my_gdextension
{

class GDEXJuceAudioPluginHostEditorWindow
    : public juce::DocumentWindow
{
public:
    explicit GDEXJuceAudioPluginHostEditorWindow(const juce::String& pluginName);
    virtual ~GDEXJuceAudioPluginHostEditorWindow() override;

private:
    //==============================================================================
    void closeButtonPressed() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GDEXJuceAudioPluginHostEditorWindow)
};

}