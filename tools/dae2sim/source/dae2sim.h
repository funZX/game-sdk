#include <dae.h>
#include <dom/domConstants.h>
#include <dom/domCOLLADA.h>

#include <iostream>
#include <filesystem>
#include <sim_engine.h>
#include <jansson.h>

// ----------------------------------------------------------------------//
using namespace ColladaDOM141;

namespace filesystem = std::experimental::filesystem::v1;

struct Options
{
	bool			animations;

	const char*		in_dae_file;
	const char*		out_folder;
} ;

extern Options options;

daeElement* daeGetUrl(daeDocument* doc, daeElement* elem);
std::vector<daeElement*> daeGetChildrenOfType(daeElement* elem, daeInt daeType);
std::vector<daeElement*> daeGetChildrenOfType(daeElement* elem, domNodeType domType);

// ----------------------------------------------------------------------//

void ArrayToJson(json_t* root, const sim::f32* v, const unsigned size);

// ----------------------------------------------------------------------//