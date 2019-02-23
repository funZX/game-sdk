#include <iostream>

#include "dae2sim.h"

void onStart();

int export_dae2sim(daeDatabase* db, const char* folder);

int main(int argc, char** argv)
{
	onStart();

	if (argc != 3)
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

	std::string path(argv[2]);

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
	//_crtBreakAlloc = 138;
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

	export_materials(db, path + "/material");
	export_meshes(db, path + "/mesh");
	export_cameras(db, path + "/camera");
	export_lights(db, path + "/light");
	export_scenes(db, path + "/scene");
	export_animations(db, path + "/animation");
	export_curves(db, path + "/curve");

	return 0;
}
// ----------------------------------------------------------------------//
