#include <iostream>

#include <render/sim_mesh.h>

#include <dae.h>

#include "dae2sim.h"

int export_curve(daeScene& scene, daeElement* curve, const std::string& path);
int export_mesh(daeScene& scene, daeElement* mesh, const std::string& path);

int export_geometry(daeScene& scene, daeElement* geom, const std::string& path)
{
    daeElement* elem = 0;
    
    elem = geom->getChild("spline");
    if (elem)
        return export_curve(scene, elem, path);

    elem = geom->getChild("mesh");
    if (elem)
        return export_mesh(scene, elem, path);

    return 0;
}