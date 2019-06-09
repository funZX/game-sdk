#include <iostream>

#include "dae2sim.h"

void onStart();
int export_dae2sim(daeDocument* doc, const char* folder);

Options options;
daeScene S;

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

	return export_dae2sim(root->getDocument(), path.c_str());
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
int export_scenes(daeDocument* doc, const std::string& path);
// ----------------------------------------------------------------------//
int export_dae2sim(daeDocument* doc, const char* folder)
{
	std::string path(folder);

	return export_scenes(doc, path);
}

// ----------------------------------------------------------------------//
daeString daeGetNodeName(daeElement* elem)
{
    return ((domNode*)elem)->getName();
}
// ----------------------------------------------------------------------//

daeElement* daeGetUrl(daeDocument* doc, daeElement* elem)
{
    std::string url = elem->getAttribute("url");
    url.erase(0, 1);

    return doc->getDatabase()->idLookup(url, doc);
}

// ----------------------------------------------------------------------//

std::vector<daeElement*> daeGetChildrenOfType(daeElement* elem, daeInt daeType)
{
    std::vector<daeElement*> ret;

    auto children = elem->getChildren();
    for (unsigned i = 0; i < children.getCount(); i++)
    {
        daeElement* child = children[i];
        domNode* dom = (domNode*)child;
        if (daeType == dom->getElementType())
            ret.push_back(child);
    }

    return ret;
}

// ----------------------------------------------------------------------//

std::vector<daeElement*> daeGetChildrenOfType(daeElement* elem, domNodeType domType)
{
    std::vector<daeElement*> ret;

    auto children = elem->getChildren();
    for (unsigned i = 0; i < children.getCount(); i++)
    {
        daeElement* child = children[i];
        domNode* dom = (domNode*)child;
        if (domType == dom->getType())
            ret.push_back(child);
    }

    return ret;
}

// ----------------------------------------------------------------------//

void ArrayToJson(json_t* root, const sim::f32* v, const unsigned count)
{
	SIM_ASSERT(root);

    for (unsigned i = 0; i < count; i++)
        json_array_append(root, json_real(v[i]));
}
// ----------------------------------------------------------------------//

