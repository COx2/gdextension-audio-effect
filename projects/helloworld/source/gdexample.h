#pragma once

#include <godot_cpp/classes/audio_effect_amplify.hpp>

namespace my_gdextension
{

class GDCustomAmplify
    : public godot::AudioEffectAmplify
{
private:
    GDCLASS(GDCustomAmplify, godot::AudioEffectAmplify)

public:
    GDCustomAmplify();
    ~GDCustomAmplify();

    //void _process(double delta) override;

protected:
    static void _bind_methods();

private:
    //double time_passed;

};

}