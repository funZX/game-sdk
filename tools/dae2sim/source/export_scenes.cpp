#include <iostream>

#include <render/scene/sim_actor.h>
#include <render/scene/sim_scene.h>

#include <dae.h>

#include "dae2sim.h"

void export_materials(daeDatabase* db, const std::string& path);

void export_scenes(daeDatabase* db, const std::string& path)
{
	export_materials(db, path + "/material");

}