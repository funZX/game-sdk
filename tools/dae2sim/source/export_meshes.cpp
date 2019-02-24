#include <iostream>

#include <render/sim_mesh.h>

#include <dae.h>

#include "dae2sim.h"

void export_meshes(daeDatabase* db, const std::string& path)
{
	std::vector<daeElement*> all_meshes = db->typeLookup(domGeometry::ID());

	daeElement* elem = NULL;

	for (daeElement* mesh : all_meshes)
	{

	}
}