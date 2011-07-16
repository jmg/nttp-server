#ifndef CONFIG_C
#define CONFIG_C

typedef struct XmlFileClass * XmlFile;

int XmlFileGetFileSize(XmlFile xmlFile);
char* XmlFileReadFile(XmlFile xmlFile, int from, int to);
char* XmlFileGetEndSection(const char* section);
char* XmlFileGetParam(XmlFile xmlFile, const char* param);
XmlFile NewXmlFile(char* path);
void FreeXmlFile(XmlFile xmlFile);
XmlFile LoadConfig(char* path);
char* SubStractString(char* string, int begin, int end);

#endif
