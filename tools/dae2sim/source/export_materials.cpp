#include <iostream>

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
		domInstance_effect* effect = (domInstance_effect*)material->getDescendant("instance_effect");
		
		elem = effect->getUrl().getElement();
		elem = elem->getDescendant("sampler2D");

		domInstance_image_Array images;
		elem->getChildrenByType(images);

		for (size_t k = 0; k < images.getCount(); ++k)
		{
			elem = images[k]->getUrl().getElement();

			domImage_source* image_source = ((domImage*)elem)->getInit_from();

			filesystem::path inPath(cdom::uriToNativePath(image_source->getRef()->getValue().str()));
			filesystem::path outPath(path);

			outPath = outPath.parent_path();
			outPath /= "texture";
			outPath /= material_name;

			filesystem::create_directories(outPath);

			outPath /= inPath.filename().string();

			filesystem::copy(inPath, outPath);

			std::cout << "Export image src:" << inPath.string() << " dst:" << outPath.string() << std::endl;
		}

		elem = effect->getDescendant("emission");
		elem = effect->getDescendant("ambient");
		elem = effect->getDescendant("diffuse");
		elem = effect->getDescendant("specular");
		elem = effect->getDescendant("shininess");




		std::cout << "Export material id:" << material->getID() << "   name:" << material_name << std::endl;
	}
}