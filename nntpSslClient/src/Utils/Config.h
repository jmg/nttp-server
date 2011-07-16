#ifndef CONFIG_H_
#define CONFIG_H_

typedef struct XmlFileClass *XmlFile;

char* MemReserveForChar(int len);
char* MemExtendForChar(char* mem, int len);
int XmlFileGetFileSize(XmlFile xmlFile);
char* XmlFileReadFile(XmlFile xmlFile, int from, int to);
char* XmlFileGetEndSection(const char* section);
char* SubString(char* string, int begin, int end);
char* XmlFileGetParam(XmlFile xmlFile, const char* param);
void FreeXmlFile(XmlFile xmlFile);
XmlFile NewXmlFile(char* path);
XmlFile LoadConfig(char* path);

#endif /* CONFIG_H_ */
