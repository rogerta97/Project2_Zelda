#include "j1XMLLoader.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "p2Log.h"

j1XMLLoader::j1XMLLoader()
{
	name = "xml";
}

j1XMLLoader::~j1XMLLoader()
{
}

void j1XMLLoader::LoadXML(const char * path, pugi::xml_document & doc)
{
	bool exist = false;

	for (std::list<xml_doc*>::iterator it = xmls.begin(); it != xmls.end(); ++it)
	{
		if (path == (*it)->path)
		{
			doc.reset((*it)->doc);
			exist = true;
			break;
		}
	}

	if(!exist)
	{
		char* buf = NULL;
		int size = App->fs->Load(path, &buf);
		pugi::xml_parse_result result = doc.load_buffer(buf, size);

		if (buf == NULL)
			LOG("Error loading '%s', probably wrong XML file name", path);
		else
		{
			LOG("Succes loading '%s'", path);
			xml_doc* new_doc = new xml_doc();
			new_doc->path = path;
			new_doc->doc.reset(doc);

			xmls.push_back(new_doc);
		}

		RELEASE(buf);
	}
}

bool j1XMLLoader::CleanUp()
{
	for (std::list<xml_doc*>::iterator it = xmls.begin(); it != xmls.end();)
	{
		RELEASE(*it);
		it = xmls.erase(it);
	}
	
	return true;
}
