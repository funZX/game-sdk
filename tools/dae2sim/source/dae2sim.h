#include <iostream>
#include <filesystem>

#include <dae.h>

#include <dom/domConstants.h>
#include <dom/domCOLLADA.h>

#include <jansson.h>
#include <math/sim_vec4.h>
// ----------------------------------------------------------------------//
using namespace ColladaDOM150;

namespace filesystem = std::experimental::filesystem::v1;

struct Options
{
	bool materials;
	bool meshes;
	bool animations;
	bool lights;
	bool cameras;
	bool curves;
	bool scenes;

	const char* in_dae_file;
	const char* out_folder;
} ;

// ----------------------------------------------------------------------//

void Vec4ToJson(json_t* root, const sim::mat::TVec4* v);

// ----------------------------------------------------------------------//