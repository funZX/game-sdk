#include <render/scene/sim_curve.h>

#include <iostream>
#include <dae.h>

#include "dae2sim.h"

int export_curve(daeScene& scene, daeElement* elem, const std::string& path)
{
    int status = 0;

    domSpline* spline = (domSpline*)elem;

    auto control = spline->getControl_vertices();    
    auto source  = control->getInput_array();

    auto x = (domSource*)daeGetSource(elem->getDocument(), source[0]); // POSITION
    auto y = (domSource*)daeGetSource(elem->getDocument(), source[1]); // IN_TANGENT
    auto z = (domSource*)daeGetSource(elem->getDocument(), source[2]); // OUT_TANGENT

    auto pp = x->getFloat_array()->getValue();
    auto pi = y->getFloat_array()->getValue();
    auto po = z->getFloat_array()->getValue();

    sim::io::CMemStream ms(32*1024);
    sim::rnr::CCurve c;

    u32 count   = (u32)x->getFloat_array()->getCount();
    for (u32 i = 0, j = 0; j < count; i++, j +=3 )
    {
        CCurve::Vertex v;
        
        v.p.x = (f32)pp.get(3 * i + 0);
        v.p.y = (f32)pp.get(3 * i + 1);
        v.p.z = (f32)pp.get(3 * i + 2);

        v.i.x = (f32)pi.get(3 * i + 0);
        v.i.y = (f32)pi.get(3 * i + 1);
        v.i.z = (f32)pi.get(3 * i + 2);

        v.o.x = (f32)po.get(3 * i + 0);
        v.o.y = (f32)po.get(3 * i + 1);
        v.o.z = (f32)po.get(3 * i + 2);

        c.AddVertex(v);
    }
    c.SetIsClosed(spline->getClosed());
    status |= !c.Save(&ms);

    daeString file = daeGetID(elem->getParent());
    std::string dir = path + "/curve/";
    filesystem::create_directories(dir);

    status |= dump(ms, dir + file);

    if (OK == status)
        scene.content.curves[daeGetName(elem->getParent())] = file;

    return status;
}