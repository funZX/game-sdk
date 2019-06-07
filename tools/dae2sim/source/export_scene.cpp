#include <iostream>

#include <render/scene/sim_actor.h>
#include <render/scene/sim_scene.h>

#include <dae.h>

#include "dae2sim.h"

// ----------------------------------------------------------------------//
int export_material(daeElement* elem, const std::string& path);
int export_animation(daeElement* animation, const std::string& path);
int export_geometry(daeDatabase* db, const std::string& path);
int export_light(daeElement* light, const std::string& path);
int export_curve(daeElement* curve, const std::string& path);
int export_camera(daeElement* camera, const std::string& path);
int export_scene(daeElement* scene, const std::string& path);

// ----------------------------------------------------------------------//

int export_scenes(daeDocument* doc, const std::string& path)
{
    daeDatabase* db = doc->getDatabase();
    daeElement*  scene = db->idLookup("scene", doc);

    if (!scene)
    {
        SIM_ERROR("No scenes found in document %s!\n", doc->getDocumentURI()->str().c_str());
        return 1;
    }

    daeTArray<daeElementRef> children = scene->getChildren();

    for (size_t i = 0; i < children.getCount(); i++)
    {
        daeElementRef child = children[i];

        if (!export_scene(child, path))
        {
            std::string url;
            child->getAttribute("url", url);
            SIM_ERROR("Failed export instance_visual_scene: %s!\n", url.c_str());

            return 1;
        }
    }
}

int export_scene(daeElement* scene, const std::string& path)
{
    return 0;
}