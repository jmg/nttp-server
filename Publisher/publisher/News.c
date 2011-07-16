#ifndef News_C
#define News_C
#include <windows.h>
#include <windows.h>
#include <strsafe.h>
#include "StrMng.h"
#include "News.h"
#include "Utils/Logs.h"

char* XmlNewGetStructure(void)
{
	return "<?xml version=\"1.0\" encoding=\"iso-8859-1\" ?><news><newsgroup>$newsgroup</newsgroup><idNoticia>$idNoticia</idNoticia><HEAD>$HEAD</HEAD><BODY>$BODY</BODY></news>";
}

XmlNew NewXmlNew(void)
{
	XmlNew xmlNew = (XmlNew) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(XmlNewClass));
	xmlNew->CurrentNew.MAXLENGTH = BODYMAX + HEADMAX + GROUPNAMEMAX;
	xmlNew->XmlStructure = MemReserveForChar(xmlNew->CurrentNew.MAXLENGTH + strlen(XmlNewGetStructure()) );
	strcpy(xmlNew->XmlStructure, XmlNewGetStructure());
	return xmlNew;
}

void FreeXmlNew(XmlNew xmlNew)
{
	MemFree(xmlNew->XmlStructure);
	MemFree(xmlNew);
}

void XmlNewPrepareXML(XmlNew xmlNew)
{
	char* CurrentIdNew;

	xmlNew->XmlStructure = Replace(xmlNew->XmlStructure, "$newsgroup", xmlNew->CurrentNew.NewsGroup);
	CurrentIdNew = LongToString(xmlNew->CurrentNew.IdNew);
	xmlNew->XmlStructure = Replace(xmlNew->XmlStructure, "$idNoticia", CurrentIdNew);	
	xmlNew->XmlStructure = Replace(xmlNew->XmlStructure, "$HEAD", xmlNew->CurrentNew.Head);
	xmlNew->XmlStructure = Replace(xmlNew->XmlStructure, "$BODY", xmlNew->CurrentNew.Body);
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se preparó el siguente xml: ", xmlNew->XmlStructure);

	MemFree(CurrentIdNew);
}

#endif