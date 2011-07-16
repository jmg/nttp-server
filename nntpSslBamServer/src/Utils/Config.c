#include "Config.h"


struct XmlFileClass {
	FILE* f;
	char* Content;
	int Size;

};

char* MemReserveForChar(int len)
{
	return (char*) calloc(len+10, sizeof(char));
}

char* MemExtendForChar(char* mem, int len)
{
	return (char*) realloc(mem, (strlen(mem) + len + 10) * sizeof(char));
}


int XmlFileGetFileSize(XmlFile xmlFile) {
	int Fsize;
	fseek(xmlFile->f, 0, SEEK_END);
	Fsize = ftell(xmlFile->f);
	rewind(xmlFile->f);
	return Fsize;
}

char* XmlFileReadFile(XmlFile xmlFile, int from, int to) {
	int len = to - from;
	char* buffer = (char*) malloc(len);
	fseek(xmlFile->f, from, SEEK_SET);
	fread(buffer, sizeof(char), len, xmlFile->f);
	buffer[len + 1] = '\0';
	return buffer;
}

char* XmlFileGetEndSection(const char* section) {
	int i;
	char* endSection = MemReserveForChar(strlen(section)+1);
	endSection[0] = '/';
	for (i = 1; section[i - 1]; i++)
		endSection[i] = section[i - 1];
	endSection[i] = '\0';
	return endSection;
}

char* SubString(char* string, int begin, int end) {
	int i, j;
	char* SubStr = MemReserveForChar(end - begin + 1);
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
	free(endParam);
	if (BeginSection < 0 || EndSection < 0)
		return NULL;

	paramValue = SubString(xmlFile->Content, BeginSection, EndSection);
	return paramValue;
}

XmlFile NewXmlFile(char* path) {
	XmlFile xmlFile = (XmlFile) malloc(sizeof(struct XmlFileClass));
	xmlFile->f = fopen(path, "r+");
	xmlFile->Size = XmlFileGetFileSize(xmlFile);
	xmlFile->Content = XmlFileReadFile(xmlFile, 0, xmlFile->Size);
	return xmlFile;
}

void FreeXmlFile(XmlFile xmlFile) {
	free(xmlFile->Content);
	free(xmlFile);
}

XmlFile LoadConfig(char* path) {
	return NewXmlFile(path);
}
