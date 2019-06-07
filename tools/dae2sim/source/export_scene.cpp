#include <iostream>

#include <render/scene/sim_actor.h>
#include <render/scene/sim_scene.h>

#include <dae.h>

#include "dae2sim.h"

// ----------------------------------------------------------------------//
int export_material(daeElement* elem, const std::string& path);
int export_animation(daeElement* animation, const std::string& path);
int export_geometry(daeElement* db, const std::string& path);
int export_mesh(daeElement* db, const std::string& path); 
int export_curve(daeElement* curve, const std::string& path);
int export_light(daeElement* light, const std::string& path);
int export_camera(daeElement* camera, const std::string& path);
int export_item(daeElement* scene, const std::string& path);
int export_scene(daeElement* scene, const std::string& path);
int export_controller(daeElement* scene, const std::string& path);

// ----------------------------------------------------------------------//

int export_scenes(daeDocument* doc, const std::string& path)
{
    daeDatabase* db     = doc->getDatabase();
    daeElement* root    = doc->getDomRoot();
    daeElement*  scene  = root->getChild("scene");

    if (!scene)
    {
        SIM_ERROR("'scene' not found in document %s!\n", doc->getDocumentURI()->str().c_str());
        return 1;
    }

    auto children = scene->getChildren();

    for (unsigned i = 0; i < children.getCount(); i++)
    {
        auto child = children[i];
        auto instance = daeGetUrl(doc, child);

        if (0 != export_scene(instance, path))
        {
            SIM_ERROR("Failed export 'instance_visual_scene': %s!\n", child->getID());
            return 1;
        }
    }

    return 0;
}

int export_scene(daeElement* scene, const std::string& path)
{
    int success = 0;

    CScene* data = SIM_NEW CScene(scene->getElementName());

    auto nodes = daeGetChildrenOfType(scene, NODETYPE_NODE);
    
    for (auto node : nodes)
    {
        node->setUserData(data);
        auto items = daeGetChildrenOfType(node, NODETYPE_NODE);
        
        for (auto item : items)
            success |= export_item(item, path);
    }

    SIM_SAFE_DELETE(data);

    return success;
}

int export_item(daeElement* item, const std::string& path)
{
    auto controller = item->getChild("instance_controller");
    if (controller)
    {
        auto instance = daeGetUrl(item->getDocument(), controller);
        instance->setUserData(item->getUserData());

        return export_controller(instance, path);
    }

    auto geometry = item->getChild("instance_geometry");
    if (geometry)
    {
        auto instance = daeGetUrl(item->getDocument(), geometry);
        instance->setUserData(item->getUserData());

        return export_geometry(instance, path);
    }

    auto camera = item->getChild("instance_camera");
    if (camera)
    {
        auto instance = daeGetUrl(item->getDocument(), camera);
        instance->setUserData(item->getUserData());

        return export_camera(instance, path);
    }

    auto light = item->getChild("instance_light");
    if (light)
    {
        auto instance = daeGetUrl(item->getDocument(), light);
        instance->setUserData(item->getUserData());

        return export_light(instance, path);
    }

    return 0;
}

int export_controller(daeElement* scene, const std::string& path)
{
    return 0;
}

