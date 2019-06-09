#include <core/io/sim_mem_stream.h>
#include <render/scene/sim_curve.h>

#include <iostream>
#include <dae.h>

#include "dae2sim.h"

int export_curve(daeElement* curve, const std::string& path)
{
    domSpline* spline = (domSpline*)curve;
    
    auto control = spline->getControl_vertices();
    
    sim::io::CMemStream ms(4096);
    sim::rnr::CCurve* c = SIM_NEW sim::rnr::CCurve();
    c->Save(&ms);
    delete c;

    return 0;
}