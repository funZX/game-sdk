#include <iostream>

#include <render/scene/sim_actor.h>
#include <render/scene/sim_scene.h>

#include <dae.h>

#include "dae2sim.h"

// ----------------------------------------------------------------------//
void export_material(daeElement* elem, const std::string& path);
void export_animation(daeElement* animation, const std::string& path);
void export_meshes(daeDatabase* db, const std::string& path);
void export_light(daeElement* light, const std::string& path);
void export_curve(daeElement* curve, const std::string& path);
void export_camera(daeElement* camera, const std::string& path);

// ----------------------------------------------------------------------//

void export_scene(daeDatabase* db, const std::string& path)
{

}