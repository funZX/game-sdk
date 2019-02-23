#include <iostream>
#include <render/sim_material.h>

#include <dae.h>

#include <dom/domMaterial.h>

#include "dae2sim.h"

void export_materials(daeDatabase* db, const std::string& path)
{
	std::vector<daeElement*> all_materials = db->typeLookup(domMaterial::ID());
	
	daeElement* elem = NULL;

	for (auto material : all_materials)
	{
		std::string material_name = material->getAttribute("name");
		domInstance_effect* effect_instance = (domInstance_effect*)material->getDescendant("instance_effect");		
		daeElement* effect = effect_instance->getUrl().getElement();

		elem = effect->getDescendant("sampler2D");

		domInstance_image_Array images;
		elem->getChildrenByType(images);

		std::vector<std::string> material_images;
		for (size_t k = 0; k < images.getCount(); ++k)
		{
			daeElement* image = images[k]->getUrl().getElement();
			domImage_source* image_source = ((domImage*)image)->getInit_from();

			filesystem::path inPath(cdom::uriToNativePath(image_source->getRef()->getValue().str()));
			filesystem::path outPath(path);

			outPath = outPath.parent_path();
			outPath /= "texture";
			outPath /= material_name;

			filesystem::create_directories(outPath);

			outPath /= inPath.filename().string();

			filesystem::copy_file(inPath, outPath, filesystem::copy_options::overwrite_existing);
			material_images.push_back(material_name + "/" + inPath.filename().string());

			std::cout << "Export image src:" << inPath.string() << " dst:" << outPath.string() << std::endl;
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


		std::cout << "Export material id:" << material->getID() << "   name:" << material_name << std::endl;
	}
}