#ifndef XmlParser_CPP
#define XmlParser_CPP

#define DllExport __declspec(dllexport)
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xmlreader.h>
#include <string>
#include <windows.h>
#include "News.cpp"
#include "Utils/Logs.h"

using namespace std;

class XmlParser
{
	private:
		New CurrentNew;

	public:
		XmlParser()
		{	
		}
		New ParseXml(BSTR xmlNew)
		{
			New CurrentNew;
						
			xmlDocPtr doc; 
			long size = SysStringLen(xmlNew) * 2;
			doc = xmlReadMemory((const char*)xmlNew, size, NULL, NULL, 0);
			if (doc == NULL) 
			{
				fprintf(stderr, "Failed to parse document\n");
				LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Failed to parse the document", NOPARMS);
				LogFileLogEndSession();
			}
			
			strcpy(CurrentNew.NewsGroup, (char*)doc->children->children->children->content);
			CurrentNew.IdNew = atol((char*) doc->children->children->next->children->content);
			strcpy(CurrentNew.Head, (char*) doc->children->children->next->next->children->content);
			strcpy(CurrentNew.Body, (char*) doc->children->children->next->next->next->children->content);
			LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se Parseo el xml", NOPARMS);
			xmlFreeDoc(doc);
			xmlCleanupParser();
			xmlMemoryDump();

			return CurrentNew;
		}
		int isValid(BSTR xmlNew)
		{
			if (!xmlNew || SysStringLen(xmlNew) < 50)
				return 0;
			return 1;
		}
};
#endif