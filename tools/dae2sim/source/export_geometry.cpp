#include <iostream>

#include <render/sim_mesh.h>

#include <dae.h>

#include "dae2sim.h"

int export_curve(daeElement* curve, const std::string& path);
int export_mesh(daeElement* mesh, const std::string& path);

int export_geometry(daeElement* geom, const std::string& path)
{
    daeElement* elem = 0;
    
    elem = geom->getChild("spline");
    if (elem)
        return export_curve(elem, path);

    elem = geom->getChild("mesh");
    if (elem)
        return export_mesh(elem, path);

    return 0;
}