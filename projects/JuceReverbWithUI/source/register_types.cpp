#include "register_types.h"

#include "juce_reverb_with_ui.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

void initialize_juce_reverb_with_ui_module(godot::ModuleInitializationLevel p_level)
{
    if (p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE)
    {
        return;
    }

    GDREGISTER_CLASS(my_gdextension::GDEXJuceReverbWithUIInstance);
    GDREGISTER_CLASS(my_gdextension::GDEXJuceReverbWithUI);
}

void uninitialize_juce_reverb_with_ui_module(godot::ModuleInitializationLevel p_level)
{
    if (p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE)
    {
        return;
    }
}

extern "C"
{

// Initialization.
GDExtensionBool GDE_EXPORT gdex_juce_reverb_with_ui_init(GDExtensionInterfaceGetProcAddress p_get_proc_address,
    const GDExtensionClassLibraryPtr p_library, 
    GDExtensionInitialization *r_initialization)
{
    godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

    init_obj.register_initializer(initialize_juce_reverb_with_ui_module);
    init_obj.register_terminator(uninitialize_juce_reverb_with_ui_module);
    init_obj.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}

}