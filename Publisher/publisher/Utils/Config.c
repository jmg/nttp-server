#include "Config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../StrMng.h"
#include <windows.h>

struct XmlFileClass {
	HANDLE f;
	char* Content;
	int Size;

};

int XmlFileGetFileSize(XmlFile xmlFile) {
	int Fsize = GetFileSize(xmlFile->f, NULL);	
	return Fsize;
}

char* XmlFileReadFile(XmlFile xmlFile, int from, int to) {
	DWORD cant;
	DWORD len = to - from;	
	LPVOID buffer = MemReserveForChar(len+2);
	int ret = ReadFile(xmlFile->f, buffer, len, &cant, NULL);
	return buffer;
}

char* XmlFileGetEndSection(const char* section) {
	int i;
	char* endSection = MemReserveForChar(strlen(section)+3);
	endSection[0] = '/';
	for (i = 1; section[i - 1]; i++)
		endSection[i] = section[i - 1];
	endSection[i] = '\0';
	return endSection;
}

char* SubStractString(char* string, int begin, int end) {
	int i, j;
	char* SubStr = MemReserveForChar(end - begin + 2);
	for (i = begin, j = 0; i <= end; i++, j++) {
		SubStr[j] = string[i];
	}
	SubStr[j] = '\0';
	return SubStr;
}

char* XmlFileGetParam(XmlFile xmlFile, const char* param) {
	int len;
	char* paramValue;
	int BeginSection = strstr(xmlFile->Content, param) - xmlFile->Content + strlen(param) + 1;
	char* endParam = XmlFileGetEndSection(param);
	int EndSection = strstr(xmlFile->Content, endParam) - xmlFile->Content - 2;
	MemFree(endParam);
	if (BeginSection < 0 || EndSection < 0)
		return NULL;

	paramValue = SubStractString(xmlFile->Content, BeginSection, EndSection);
	return paramValue;
}

XmlFile NewXmlFile(char* path) {
	XmlFile xmlFile = (XmlFile) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(struct XmlFileClass));
	xmlFile->f = CreateFileA(path, FILE_READ_ACCESS, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	xmlFile->Size = XmlFileGetFileSize(xmlFile);
	xmlFile->Content = XmlFileReadFile(xmlFile, 0, xmlFile->Size);	
	return xmlFile;
}

void FreeXmlFile(XmlFile xmlFile) {
	CloseHandle(xmlFile->f);
	MemFree(xmlFile->Content);
	MemFree(xmlFile);
}

XmlFile LoadConfig(char* path) {
	return NewXmlFile(path);
}
