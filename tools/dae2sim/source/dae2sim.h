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
    bool            display;

	const char*		in_dae_file;
	const char*		out_folder;
} ;

extern Options options;

daeElement* daeGetSource(daeDocument* doc, daeElement* elem);
daeElement* daeGetUrl(daeDocument* doc, daeElement* elem);
daeString daeGetNodeName(daeElement* elem);
std::vector<daeElement*> daeGetChildrenOfType(daeElement* elem, daeInt daeType);
std::vector<daeElement*> daeGetChildrenOfType(daeElement* elem, domNodeType domType);


struct daeSceneNode
{
    daeElement*                             elem;
    std::map<std::string, daeSceneNode*>    childs;
};

struct daeContent
{
    std::map<std::string, daeElement*>      materials;
    std::map<std::string, daeElement*>      lights;
    std::map<std::string, daeElement*>      cameras;
    std::map<std::string, daeElement*>      curves;
    std::map<std::string, daeElement*>      meshes;
    std::map<std::string, daeElement*>      controllers;
    std::map<std::string, daeElement*>      animations;
    std::map<std::string, daeElement*>      animationclips;
};

struct daeScene
{
    daeSceneNode                            root;
    daeContent                              content;
};

// ----------------------------------------------------------------------//

void ArrayToJson(json_t* root, const sim::f32* v, const unsigned size);

// ----------------------------------------------------------------------//