#ifndef _J1XMLLOADER_H_
#define _J1XMLLOADER_H_

#include "j1Module.h"

struct xml_doc
{
	pugi::xml_document	doc;
	string				path;
};

class j1XMLLoader :public j1Module
{
public:
	j1XMLLoader();
	~j1XMLLoader();

	void LoadXML(const char* path, pugi::xml_document& doc);

	// Called before quitting
	bool CleanUp();

	

private:

public:

private:

	std::list<xml_doc*>	xmls;
};

#endif // !_J1XMLLOADER_H_

