#include "gdexample.h"
#include <godot_cpp/core/class_db.hpp>

namespace my_gdextension
{

GDCustomAmplify::GDCustomAmplify()
{
    // Initialize any variables here.
    set_volume_db(-8.2f);
}

GDCustomAmplify::~GDCustomAmplify()
{
    // Add your cleanup here.
}

void GDCustomAmplify::_bind_methods()
{
}

}