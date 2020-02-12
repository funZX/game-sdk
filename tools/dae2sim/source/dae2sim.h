#include <dae.h>
#include <dom/domConstants.h>
#include <dom/domCOLLADA.h>

#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <jansson.h>
#include <sim_engine.h>
#include <core/io/sim_mem_stream.h>


// ----------------------------------------------------------------------//
#define OK          (0)
// ----------------------------------------------------------------------//
using namespace ColladaDOM141;

namespace filesystem = std::experimental::filesystem::v1;

struct Options
{
    bool            display;

	const char*		in_dae_file;
	const char*		out_folder;
} ;

extern Options options;

daeElement* daeGetSource(daeDocument* doc, daeElement* elem);
daeElement* daeGetUrl(daeDocument* doc, daeElement* elem);
daeString daeGetName(daeElement* elem);
daeString daeGetID(daeElement* elem);
std::vector<daeElement*> daeGetChildrenOfType(daeElement* elem, daeInt daeType);
std::vector<daeElement*> daeGetChildrenOfType(daeElement* elem, domNodeType domType);

int dump(CMemStream& ms, const std::string& file);

struct daeSceneNode
{
    daeElement*                             elem;
    std::map<std::string, daeSceneNode>     childs;
};

struct daeContent
{
    std::map<std::string, std::string>      materials;
    std::map<std::string, std::string>      lights;
    std::map<std::string, std::string>      cameras;
    std::map<std::string, std::string>      curves;
    std::map<std::string, std::string>      meshes;
};

struct daeScene
{
    daeSceneNode                            root;
    daeContent                              content;
};

// ----------------------------------------------------------------------//

void ArrayToJson(json_t* root, const sim::f32* v, const unsigned size);

// ----------------------------------------------------------------------//