#ifndef CONFIG_C
#define CONFIG_C
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct XmlFileClass * XmlFile;

char* MemReserveForChar(int len);
char* SubString(char* string, int begin, int end);
char* MemExtendForChar(char* mem, int len);

int XmlFileGetFileSize(XmlFile xmlFile);
char* XmlFileReadFile(XmlFile xmlFile, int from, int to);
char* XmlFileGetEndSection(const char* section);
char* XmlFileGetParam(XmlFile xmlFile, const char* param);
XmlFile NewXmlFile(char* path);
void FreeXmlFile(XmlFile xmlFile);
XmlFile LoadConfig(char* path);
char* SubString(char* string, int begin, int end);

#endif
