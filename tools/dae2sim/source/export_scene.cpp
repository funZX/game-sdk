#include <iostream>

#include <render/scene/sim_scene.h>

#include <dae.h>
#include "dae2sim.h"

// ----------------------------------------------------------------------//
int export_material(daeElement* material, const std::string& path);
int export_animation(daeElement* animation, const std::string& path);
int export_animationclip(daeElement* animationclip, const std::string& path);
int export_geometry(daeElement* geom, const std::string& path);
int export_mesh(daeElement* mesh, const std::string& path);
int export_curve(daeElement* curve, const std::string& path);
int export_light(daeElement* light, const std::string& path);
int export_camera(daeElement* camera, const std::string& path);
int export_controller(daeElement* controller, const std::string& path);
// ----------------------------------------------------------------------//



// ----------------------------------------------------------------------//
int export_node(daeSceneNode& item, const std::string& path);
int export_scene(daeElement* elem, const std::string& path);
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

int export_scene(daeElement* elem, const std::string& path)
{
    int success = 0;

    daeScene s;
    s.root.elem = elem;

    auto nodes = daeGetChildrenOfType(elem, NODETYPE_NODE);
    
    for (auto node : nodes)
    {
        daeSceneNode n;
        n.elem = node;

        success |= export_node(n, path);

        s.root.childs[daeGetName(node)] = n;
    }

    sim::io::CMemStream ms(1024*1024);
    sim::rnr::CScene scn;

    success |= !scn.Save(&ms);

    std::string dir = path + "/scene/";
    filesystem::create_directories(dir);

    success |= dump(ms, dir + daeGetName(elem));

    return success;
}

int export_node(daeSceneNode& item, const std::string& path)
{
    int success = 0;
    auto nodes  = daeGetChildrenOfType(item.elem, NODETYPE_NODE);

    for (auto node : nodes)
    {
        daeSceneNode n;
        n.elem = node;

        success |= export_node(n, path);

        item.childs[daeGetName(node)] = n;
    }

    auto controller = item.elem->getChild("instance_controller");
    if (controller)
    {
        auto instance = daeGetUrl(item.elem->getDocument(), controller);
        return export_controller(instance, path);
    }

    auto geometry = item.elem->getChild("instance_geometry");
    if (geometry)
    {
        auto instance = daeGetUrl(item.elem->getDocument(), geometry);
        return export_geometry(instance, path);
    }

    auto camera = item.elem->getChild("instance_camera");
    if (camera)
    {
        auto instance = daeGetUrl(item.elem->getDocument(), camera);
        return export_camera(instance, path);
    }

    auto light = item.elem->getChild("instance_light");
    if (light)
    {
        auto instance = daeGetUrl(item.elem->getDocument(), light);
        return export_light(instance, path);
    }

    return success;
}
