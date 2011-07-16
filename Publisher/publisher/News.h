;

#define BODYMAX 10000
#define HEADMAX 10000
#define GROUPNAMEMAX 10000

typedef struct _NewClass
{
    long IdNew;
    char NewsGroup[BODYMAX];
    char Head[HEADMAX];
    char Body[GROUPNAMEMAX];
    int Transmitted;
	int MAXLENGTH;

} New;

typedef struct _XmlNewClass
{
	New CurrentNew;
    char* XmlStructure;

} XmlNewClass, *XmlNew;

char* XmlNewGetStructure(void);
XmlNew NewXmlNew(void);
void FreeXmlNew(XmlNew xmlNew);
void XmlNewPrepareXML(XmlNew xmlNew);
