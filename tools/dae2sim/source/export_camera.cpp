#include <iostream>
#include <dae.h>

#include <render/scene/sim_camera.h>

#include "dae2sim.h"

int export_camera(daeScene& scene, daeElement* elem, const std::string& path)
{
    int status = 0;

    auto perspective = ((domCamera*)elem)->getOptics()->getTechnique_common()->getPerspective();

    sim::io::CMemStream ms(1024);
    sim::rnr::CCamera camera;

    camera.SetFieldOfView((f32)perspective->getYfov()->getValue());
    camera.SetNearPlane((f32)perspective->getZnear()->getValue());
    camera.SetFarPlane((f32)perspective->getZfar()->getValue());

    status |= !camera.Save(&ms);

    daeString file = daeGetID(elem);
    std::string dir = path + "/camera/";
    filesystem::create_directories(dir);

    status |= dump(ms, dir + file);

    if (OK == status)
        scene.content.cameras[daeGetName(elem)] = file;

    return status;
}