#include <iostream>
#include <render/sim_material.h>

#include <dae.h>

#include <dom/domMaterial.h>

#include "dae2sim.h"

void export_material(daeElement* material, const std::string& path)
{
    /*
    std::string material_name = material->getAttribute("name");
    domInstance_effect* effect_instance = (domInstance_effect*)material->getDescendant("instance_effect");
    daeElement* effect = effect_instance->getUrl().getElement();

    std::vector<std::string> material_textures;
    elem = effect->getDescendant("sampler2D");
    if (elem)
    {
        domInstance_image_Array images;
        elem->getChildrenByType(images);

        for (size_t k = 0; k < images.getCount(); ++k)
        {
            daeElement* image = images[k]->getUrl().getElement();
            domImage_source* image_source = ((domImage*)image)->getInit_from();

            filesystem::path inPath(cdom::uriToNativePath(image_source->getRef()->getValue().str()));
            filesystem::path outPath(path);

            outPath = outPath.parent_path();
            outPath /= "texture";

            filesystem::create_directories(outPath);

            outPath /= inPath.filename().string();

            filesystem::copy_file(inPath, outPath, filesystem::copy_options::overwrite_existing);
            material_textures.push_back(inPath.filename().string());

            std::cout << "Export image src:" << inPath.string() << " dst:" << outPath.string() << std::endl;
        }
    }

    domFx_common_color_or_texture* color = NULL;
    domFx_common_float_or_param* value = NULL;
    sim::rnr::CMaterial engine_material;

    elem = effect->getDescendant("emission");
    if (elem->getDescendant("color"))
    {
        TVec4 material_emission;
        color = (domFx_common_color_or_texture*)elem;
        domFx_color emission = color->getColor()->getValue();
        Vec4Set(&material_emission, emission[0], emission[1], emission[2], emission[3]);
        engine_material.SetEmissive(&material_emission);
    }

    elem = effect->getDescendant("ambient");
    if (elem->getDescendant("color"))
    {
        TVec4 material_ambient;
        color = (domFx_common_color_or_texture*)elem;
        domFx_color ambient = color->getColor()->getValue();
        Vec4Set(&material_ambient, ambient[0], ambient[1], ambient[2], ambient[3]);
        engine_material.SetAmbient(&material_ambient);
    }

    elem = effect->getDescendant("diffuse");
    if (elem->getDescendant("color"))
    {
        TVec4 material_diffuse;
        color = (domFx_common_color_or_texture*)elem;
        domFx_color diffuse = color->getColor()->getValue();
        Vec4Set(&material_diffuse, diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
        engine_material.SetDiffuse(&material_diffuse);
    }

    elem = effect->getDescendant("specular");
    if (elem->getDescendant("color"))
    {
        TVec4 material_specular;
        color = (domFx_common_color_or_texture*)elem;
        domFx_color specular = color->getColor()->getValue();
        Vec4Set(&material_specular, specular[0], specular[1], specular[2], specular[3]);
        engine_material.SetSpecular(&material_specular);
    }

    elem = effect->getDescendant("shininess");
    if (elem->getDescendant("float"))
    {
        value = (domFx_common_float_or_param*)elem;
        engine_material.SetShininess(value->getFloat()->getValue());
    }

    elem = effect->getDescendant("index_of_refraction");
    if (elem->getDescendant("float"))
    {
        value = (domFx_common_float_or_param*)elem;
        engine_material.SetRefraction(value->getFloat()->getValue());
    }

    json_t* json_root = json_object();
    json_t* json_ambient = json_array();
    json_t* json_diffuse = json_array();
    json_t* json_specular = json_array();
    json_t* json_emissive = json_array();
    json_t* json_reflective = json_array();

    Vec4ToJson(json_ambient, engine_material.GetAmbient());
    json_object_set(json_root, "ambient", json_ambient);

    Vec4ToJson(json_diffuse, engine_material.GetDiffuse());
    json_object_set(json_root, "diffuse", json_diffuse);

    Vec4ToJson(json_specular, engine_material.GetSpecular());
    json_object_set(json_root, "specular", json_specular);

    Vec4ToJson(json_emissive, engine_material.GetEmissive());
    json_object_set(json_root, "emissive", json_emissive);

    Vec4ToJson(json_reflective, engine_material.GetReflective());
    json_object_set(json_root, "reflective", json_reflective);

    json_object_set(json_root, "shininess", json_real(engine_material.GetShininess()));
    json_object_set(json_root, "refraction", json_real(engine_material.GetRefraction()));
    json_object_set(json_root, "effect", json_string("default"));

    json_t* json_texture = json_array();
    for (unsigned int k = 0; k < material_textures.size(); ++k)
    {
        json_t* texture = json_object();
        json_object_set(texture, "channel", json_integer(k));
        json_object_set(texture, "name", json_string(material_textures[k].c_str()));

        json_array_insert(json_texture, k, texture);
    }

    json_object_set(json_root, "texture", json_texture);


    filesystem::path material_path(path);
    filesystem::create_directories(material_path);
    material_path /= material_name + ".json";

    json_dump_file(json_root, material_path.string().c_str(), 0);

    std::cout << "Export material id:" << material->getID() << "   name:" << material_name << std::endl;
    */
}