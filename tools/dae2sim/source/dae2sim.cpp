#include <iostream>

#include "dae2sim.h"

void onStart();

int export_dae2sim(daeElement* root, const char* folder);

int main(int argc, char** argv)
{
	onStart();

	if (argc < 3)
	{
		std::cout << "dae2sim INPUT_FILE_V1.5.dae OUTPUT_FOLDER";
		return 0;
	}

	DAE dae;
	daeElement* root = dae.open(argv[1]);

	if (!root)
	{
		std::cout << "Document import failed.\n";
		return 0;
	}

	return export_dae2sim(root, argv[2]);
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
	//_crtBreakAlloc = 138;
#endif

	atexit(onQuit);
}

// ----------------------------------------------------------------------//
void export_materials(const daeElement* root, const char* folder);
void export_meshes(const daeElement* root, const char* folder);
void export_cameras(const daeElement* root, const char* folder);
void export_lights(const daeElement* root, const char* folder);
void export_scenes(const daeElement* root, const char* folder);
void export_animations(const daeElement* root, const char* folder);
void export_curves(const daeElement* root, const char* folder);

// ----------------------------------------------------------------------//
int export_dae2sim(daeElement* root, const char* folder)
{
	export_materials(root, folder);
	export_meshes(root, folder);
	export_cameras(root, folder);
	export_lights(root, folder);
	export_scenes(root, folder);
	export_animations(root, folder);
	export_curves(root, folder);

	return 0;
}
// ----------------------------------------------------------------------//
