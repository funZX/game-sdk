#include <render/scene/sim_curve.h>

#include <iostream>
#include <dae.h>

#include "dae2sim.h"

int export_curve(daeElement* curve, const std::string& path)
{
    int success = 0;

    domSpline* spline = (domSpline*)curve;

    auto control = spline->getControl_vertices();    
    auto source  = control->getInput_array();

    auto x = (domSource*)daeGetSource(curve->getDocument(), source[0]); // POSITION
    auto y = (domSource*)daeGetSource(curve->getDocument(), source[1]); // IN_TANGENT
    auto z = (domSource*)daeGetSource(curve->getDocument(), source[2]); // OUT_TANGENT

    auto pp = x->getFloat_array()->getValue();
    auto pi = y->getFloat_array()->getValue();
    auto po = z->getFloat_array()->getValue();

    sim::io::CMemStream ms(32*1024);
    sim::rnr::CCurve c;

    u32 count   = x->getFloat_array()->getCount();
    for (auto i = 0, j = 0; j < count; i++, j +=3 )
    {
        CCurve::Vertex v;
        
        v.p.x = pp.get(3 * i + 0);
        v.p.y = pp.get(3 * i + 1);
        v.p.z = pp.get(3 * i + 2);

        v.i.x = pi.get(3 * i + 0);
        v.i.y = pi.get(3 * i + 1);
        v.i.z = pi.get(3 * i + 2);

        v.o.x = po.get(3 * i + 0);
        v.o.y = po.get(3 * i + 1);
        v.o.z = po.get(3 * i + 2);

        c.AddVertex(v);
    }
    c.SetIsClosed(spline->getClosed());
    success |= !c.Save(&ms);

    std::string dir = path + "/curve/";
    filesystem::create_directories(dir);

    success |= dump(ms, dir + daeGetName(spline->getParent()));

    return success;
}