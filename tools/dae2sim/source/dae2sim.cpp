#include <iostream>

#include "dae2sim.h"

void onStart();
int export_dae2sim(daeDatabase* db, const char* folder);

Options options;

int main(int argc, char** argv)
{
	onStart();

	options.materials = false;
	options.meshes = false;
	options.animations = false;
	options.lights = false;
	options.cameras = false;
	options.curves = false;
	options.scenes = false;

	options.in_dae_file = 0;
	options.out_folder = 0;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp("-material", argv[i]) == 0)
		{
			if (i + 1 == argc) break;
			i++;

			options.materials = true;
		}
		if (strcmp("-mesh", argv[i]) == 0)
		{
			if (i + 1 == argc) break;
			i++;

			options.meshes = true;
		}
		if (strcmp("-animation", argv[i]) == 0)
		{
			if (i + 1 == argc) break;
			i++;

			options.animations = true;
		}
		if (strcmp("-light", argv[i]) == 0)
		{
			if (i + 1 == argc) break;
			i++;

			options.lights = true;
		}
		if (strcmp("-camera", argv[i]) == 0)
		{
			if (i + 1 == argc) break;
			i++;

			options.cameras = true;
		}
		if (strcmp("-curve", argv[i]) == 0)
		{
			if (i + 1 == argc) break;
			i++;

			options.curves = true;
		}
		if (strcmp("-scene", argv[i]) == 0)
		{
			if (i + 1 == argc) break;
			i++;

			options.scenes = true;
		}
		else if (argv[i][0] != '-')
		{
			options.in_dae_file = argv[i];

			if (i + 1 < argc && argv[i + 1][0] != '-') {
				options.out_folder = argv[i + 1];
			}

			break;
		}
	}

	if (!(options.in_dae_file && options.out_folder))
	{
		printf("_____________________________________________________________________\n");
		printf("|                  Copyright (c) Adrian SIMINCIUC 2019               |\n");
		printf("|                       Authors: Adrian SIMINCIUC                    |\n");
		printf("|____________________________________________________________________|\n\n");
		printf("usage: dae2sim [options] input_dae_file_v1.5 output_folder\n\n");
		printf("Options:\n");
		printf("  -material\n");
		printf("  -mesh\n");
		printf("  -animation\n");
		printf("  -light\n");
		printf("  -camera\n");
		printf("  -curve\n");
		printf("  -scene\n");
		printf("_____________________________________________________________________\n");
		printf("|  *****************     IMPORTANT NOTES      **********************|\n");
		printf("|___________________________________________________________________|\n");
		printf("|         If no option provided nothing is exported but             |\n");
		printf("|             the document is checked for import.                   |\n");
		printf("|___________________________________________________________________|\n");

		return 0;
	}
	
	DAE dae;
	daeElement* root = dae.open(options.in_dae_file);

	if (!root)
	{
		std::cout << "Document import failed.\n";
		return 0;
	}

	std::string path(options.out_folder);

	return export_dae2sim(root->getDocument()->getDatabase(), path.c_str());
}


// ----------------------------------------------------------------------//

void onQuit()
{
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
}

void onStart()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_crtBreakAlloc = 72672;
#endif

	atexit(onQuit);
}

// ----------------------------------------------------------------------//
void export_materials(daeDatabase* db, const std::string& path);
void export_meshes(daeDatabase* db, const std::string& path);
void export_cameras(daeDatabase* db, const std::string& path);
void export_lights(daeDatabase* db, const std::string& path);
void export_scenes(daeDatabase* db, const std::string& path);
void export_animations(daeDatabase* dbt, const std::string& path);
void export_curves(daeDatabase* db, const std::string& path);

// ----------------------------------------------------------------------//
int export_dae2sim(daeDatabase* db, const char* folder)
{
	std::string path(folder);

	if (options.materials)
		export_materials(db, path + "/material");

	if (options.meshes)
		export_meshes(db, path + "/mesh");

	if (options.animations)
		export_animations(db, path + "/animation");

	if (options.cameras)
		export_cameras(db, path + "/camera");

	if (options.lights)
		export_lights(db, path + "/light");

	if (options.scenes)
		export_scenes(db, path + "/scene");

	if (options.curves)
		export_curves(db, path + "/curve");

	return 0;
}
// ----------------------------------------------------------------------//

void Vec4ToJson(json_t* root, const sim::mat::TVec4* v)
{
	SIM_ASSERT(root);

	json_array_insert(root, 0, json_real(v->x));
	json_array_insert(root, 1, json_real(v->y));
	json_array_insert(root, 2, json_real(v->z));
	json_array_insert(root, 3, json_real(v->w));
}
// ----------------------------------------------------------------------//

