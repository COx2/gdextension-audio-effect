#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class VST3PluginLoader
{
public:
    // Load a VST3 plugin from a file path
    static std::unique_ptr<juce::AudioPluginInstance> loadVST3FromPath(const juce::String& fileOrBundlePath)
    {
        // Make sure the file exists
        juce::File pluginFileOrBundle(fileOrBundlePath);
        if (!pluginFileOrBundle.exists())
        {
            juce::String errorMessage = "Plugin file does not exist: " + fileOrBundlePath;
            juce::Logger::writeToLog(errorMessage);
            return nullptr;
        }
        
        // Check if the file has a VST3 extension
        if (pluginFileOrBundle.existsAsFile() && !pluginFileOrBundle.getFileExtension().equalsIgnoreCase(".vst3"))
        {
            juce::String errorMessage = "File is not a VST3 plugin: " + fileOrBundlePath;
            juce::Logger::writeToLog(errorMessage);
            return nullptr;
        }
        else if (pluginFileOrBundle.isDirectory() && !pluginFileOrBundle.getFileExtension().equalsIgnoreCase(".vst3"))
        {
            juce::String errorMessage = "Bundle is not a VST3 plugin: " + fileOrBundlePath;
            juce::Logger::writeToLog(errorMessage);
            return nullptr;
        }
        
        // Create the plugin format manager
        juce::AudioPluginFormatManager formatManager;
        formatManager.addDefaultFormats(); // Adds VST3, AU, etc. formats
        
        // Find the VST3 format
        juce::AudioPluginFormat* vst3Format = nullptr;
        for (int i = 0; i < formatManager.getNumFormats(); ++i)
        {
            auto* format = formatManager.getFormat(i);
            if (format->getName() == "VST3")
            {
                vst3Format = format;
                break;
            }
        }
        
        if (vst3Format == nullptr)
        {
            juce::String errorMessage = "VST3 format not found";
            juce::Logger::writeToLog(errorMessage);
            return nullptr;
        }
        
        // Create a description for the plugin we want to load
        juce::OwnedArray<juce::PluginDescription> descriptions;
        vst3Format->findAllTypesForFile(descriptions, pluginFileOrBundle.getFullPathName());
        
        if (descriptions.isEmpty())
        {
            juce::String errorMessage = "No plugin found in file: " + fileOrBundlePath;
            juce::Logger::writeToLog(errorMessage);
            return nullptr;
        }
        
        // Create result object to store any error messages
        juce::String errorMessage;
        
        // Create the plugin instance first listed
        std::unique_ptr<juce::AudioPluginInstance> instance = 
            formatManager.createPluginInstance(*descriptions[0], 44100.0, 512, errorMessage);
        
        if (instance == nullptr)
        {
            juce::String logMessage = "Failed to create plugin instance: " + errorMessage;
            juce::Logger::writeToLog(logMessage);
            return nullptr;
        }
        
        return instance;
    }
};

#if 0
// Example usage:
void loadAndUsePlugin()
{
    // Path to your VST3 plugin
    juce::String pluginPath = "/path/to/your/plugin.vst3";
    
    // Load the plugin
    std::unique_ptr<juce::AudioPluginInstance> pluginInstance = 
        VST3PluginLoader::loadVST3FromPath(pluginPath);
    
    if (pluginInstance != nullptr)
    {
        // The plugin was loaded successfully!
        
        // You can now access the AudioProcessor
        juce::AudioProcessor* processor = pluginInstance.get();
        
        // Now you can use the processor as needed
        // For example, you might prepare it for playback:
        processor->prepareToPlay(44100.0, 512);
        
        // Or get its parameters:
        for (auto i = 0; i < processor->getNumParameters(); ++i)
        {
            auto name = processor->getParameterName(i);
            auto value = processor->getParameterValue(i);
            
            // Do something with the parameter...
        }
        
        // And so on...
    }
    else
    {
        // Failed to load the plugin
        juce::Logger::writeToLog("Failed to load plugin");
    }
}
#endif
