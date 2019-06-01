#include <iostream>

#include "dae2sim.h"

void onStart();
int export_dae2sim(daeDatabase* db, const char* folder);

Options options;

int main(int argc, char** argv)
{
	onStart();

	options.in_dae_file = 0;
	options.out_folder = 0;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp("-animation", argv[i]) == 0)
		{
			if (i + 1 == argc) break;
			i++;

			options.animations = true;
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
		printf("USAGE: dae2sim input_dae_file_v1.5 output_folder\n\n");


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
	//_CrtDumpMemoryLeaks();
#endif
}

void onStart()
{
#ifdef _DEBUG
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_crtBreakAlloc = 72672;
#endif

	atexit(onQuit);
}

// ----------------------------------------------------------------------//
void export_scenes(daeDatabase* db, const std::string& path);
// ----------------------------------------------------------------------//
int export_dae2sim(daeDatabase* db, const char* folder)
{
	std::string path(folder);

	export_scenes(db, path);

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

