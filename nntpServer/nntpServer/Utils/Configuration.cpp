#ifndef CONFIG_CPP
#define CONFIG_CPP

#include "configuration.h"
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>

struct XmlFileClass {
	HANDLE f;
	char* Content;
	int Size;

};

typedef struct XmlFileClass * XmlFile;

char* MemReserveForChar(int len)
{
	return (char*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (len+1) * sizeof(char));
}

char* MemExtendForChar(char* mem, int len)
{
	return (char*) HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, mem,sizeof(char) * (len+1));
}

void MemFree(void* mem)
{
    HeapFree(GetProcessHeap(), 0, mem);
}

int XmlFileGetFileSize(XmlFile xmlFile) {
	int Fsize = GetFileSize(xmlFile->f, NULL);	
	return Fsize;
}

char* XmlFileReadFile(XmlFile xmlFile, int from, int to) {
	DWORD cant;
	DWORD len = to - from;	
	LPVOID buffer = MemReserveForChar(len+2);
	int ret = ReadFile(xmlFile->f, buffer, len, &cant, NULL);
	return (char*)buffer;
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
#endif