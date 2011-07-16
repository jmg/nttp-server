#include "C:\Program Files\Oracle\Berkeley DB 4.8.26\include\db.h"


#define NOUNEXPORTEDNEWS -100
#define DBCLOSED -101
#define OVERRIDE 1
#define NOVERRIDE 0
#define EXPORTED 1
#define NOEXPORTED 0

typedef struct _DataBaseClass
{
	DB* dbp; 
	u_int32_t flags;
	int status;
	DBC* cursor;
	WCHAR* pathUnicode;
	char* path;

} DataBaseClass, *DataBase;

DataBase dataBase;

DataBase NewDataBase(void);
DataBase FreeDataBase(void);
void CreateOrOpenDataBase(char* path);
void OpenBataDase(void);
void CloseDataBase(void);

typedef struct _NewRowClass
{	
	DBT key;
	DBT data;	
} NewRowClass, *NewRow;

NewRow NewNewRow(void);
void FreeNewRow(NewRow newRow);
void SaveInDataBase(New NewToInsert);
New ScanDataBase(void);
New DataBaseGetFirstNoExported();
New DataBaseGetById(long id);
void UpdateDataBase(New CurrentNew);
void DataBaseSave(New NewToInsert, int Override);
