#ifndef CONFIG_H
#define CONFIG_H

typedef struct XmlFileClass * XmlFile;

int XmlFileGetFileSize(XmlFile xmlFile);
char* XmlFileReadFile(XmlFile xmlFile, int from, int to);
char* XmlFileGetEndSection(const char* section);
char* XmlFileGetParam(XmlFile xmlFile, const char* param);
XmlFile NewXmlFile(char* path);
void FreeXmlFile(XmlFile xmlFile);
XmlFile LoadConfig(char* path);
char* SubStractString(char* string, int begin, int end);
char* MemReserveForChar(int len);
char* MemExtendForChar(char* mem, int len);
void MemFree(void* mem);


#endif
