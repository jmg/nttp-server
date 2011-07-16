#ifndef Database_C
#define Database_C
#include <windows.h>
#include "News.h"
#include "DataBase.h"
#include "Utils/Logs.h"

DataBase NewDataBase(void)
{
	return (DataBase) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(DataBaseClass));
}

DataBase FreeDataBase(void)
{
	HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, dataBase->path);
	HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, dataBase);
}

int ExistDataBase(void)
{
	WIN32_FIND_DATA findData;
	HANDLE hFind;	
	ZeroMemory(&findData, sizeof(findData));
	hFind = FindFirstFile(dataBase->pathUnicode, &findData);
	if (hFind == INVALID_HANDLE_VALUE)
		return 0;
	return 1;
}

void CreateOrOpenDataBase(char* path)
{	
	dataBase = NewDataBase();
		
	if (!(dataBase->status = db_create(&dataBase->dbp, NULL, 0)))
	{
		if (!(dataBase->status = dataBase->dbp->open(dataBase->dbp, NULL, path, NULL, DB_BTREE, DB_CREATE | DB_THREAD, 0644)))
		{
			LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se creo la base de datos Berkley en: ", path);
			dataBase->pathUnicode = MemReserveForChar(strlen(path)+1);
			dataBase->path = MemReserveForChar(strlen(path)+1);
			dataBase->pathUnicode = MemReserveForChar(strlen(path)+1);
			strcpy(dataBase->path, path);
			AnsiToUnicode(path, &dataBase->pathUnicode);				
		}
	}	
}

void CreateIfNotExistsDataBase(void)
{
	if(!ExistDataBase())
	{	
		if (!(dataBase->status = db_create(&dataBase->dbp, NULL, 0)))
		{
			if (!(dataBase->status = dataBase->dbp->open(dataBase->dbp, NULL, dataBase->path, NULL, DB_BTREE, DB_CREATE | DB_THREAD, 0644)))
			{
				LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se creo la base de datos Berkley en: ", dataBase->path);				
			}
		}	
	}
}

void OpenDataBase(void)
{  		
	dataBase->dbp->open(dataBase->dbp, NULL, dataBase->path, NULL, DB_BTREE, 0, 0644);
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se abrió una conexión a la base de datos", NOPARMS);
}

void CloseDataBase(void)
{
	dataBase->dbp->close(dataBase->dbp, 0);
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se cerró la conexión a la base de datos", NOPARMS);
	FreeDataBase(dataBase);
}

NewRow NewNewRow(void)
{
	NewRow newRow = (NewRow) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(NewRowClass));;
	memset(&newRow->key, 0, sizeof(DBT)); 
	memset(&newRow->data, 0, sizeof(DBT)); 
	newRow->key.size = sizeof(long);
	newRow->data.size = sizeof(New);	
	return newRow;
}

void FreeNewRow(NewRow newRow)
{
	HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, newRow);
}

void SaveInDataBase(New NewToInsert)
{
	NewToInsert.Transmitted = NOEXPORTED;
	DataBaseSave(NewToInsert, NOVERRIDE);
}

New ScanDataBase(void)
{
	return DataBaseGetFirstNoExported();
}

void UpdateDataBase(New CurrentNew)
{
	CurrentNew.Transmitted = EXPORTED;
	DataBaseSave(CurrentNew, OVERRIDE);
}

void DataBaseSave(New NewToInsert, int Override) 
{
	NewRow newRow = NewNewRow();
	long flag = Override ? 0 : DB_NOOVERWRITE;

	newRow->key.data = &NewToInsert.IdNew;
	newRow->data.data = &NewToInsert;

	CreateIfNotExistsDataBase();
	dataBase->status = dataBase->dbp->put(dataBase->dbp, NULL, &newRow->key, &newRow->data, flag);
	if (dataBase->status == DB_KEYEXIST) 
	{
		dataBase->dbp->err(dataBase->dbp, dataBase->status, "La clave ya existe!\n");
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "la clave ya existe", NOPARMS);
	}
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se guardo una noticia en la base de datos", NOPARMS);
	FreeNewRow(newRow);
}

New DataBaseGetFirstNoExported(void)
{
	NewRow newRow = NewNewRow();
	New CurrentNew;
	long key;

	newRow->key.data = &key;
	newRow->data.data = &CurrentNew;	
	newRow->data.ulen = sizeof(New);
	newRow->data.flags = DB_DBT_USERMEM;
	
	if ((dataBase->status = dataBase->dbp->cursor(dataBase->dbp, NULL, &dataBase->cursor, 0)) != 0) 
	{
		if(dataBase->status == DB_NOTFOUND)
		{
			CurrentNew.IdNew = DB_NOTFOUND;
		}
		printf("%s",db_strerror(dataBase->status));
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Error armando el cursor de datos", NOPARMS);
	}
	for(dataBase->status = dataBase->cursor->get(dataBase->cursor, &newRow->key, &newRow->data, DB_FIRST);
		dataBase->status == 0;
		dataBase->status = dataBase->cursor->get(dataBase->cursor, &newRow->key, &newRow->data, DB_NEXT)
		)
	{
		if(CurrentNew.Transmitted == NOEXPORTED)
		{
			LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se devolvió una noticia sin exportar", NOPARMS);
			return CurrentNew;
		}
	}
	CurrentNew.IdNew = NOUNEXPORTEDNEWS;
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "No hay mas noticias sin exportar", NOPARMS);
	return CurrentNew;
}

New DataBaseGetById(long id)
{
	NewRow newRow = NewNewRow();
	New CurrentNew;

	long key = id;
	newRow->key.data = &key;
	newRow->data.data = &CurrentNew;	
	newRow->data.ulen = sizeof(New);
	newRow->data.flags = DB_DBT_USERMEM;
	
	if ((dataBase->status = dataBase->dbp->get(dataBase->dbp, NULL, &newRow->key, &newRow->data, 0)) != 0) 
	{
		if (dataBase->status == DB_NOTFOUND)
		{
			CurrentNew.IdNew = DB_NOTFOUND;
		}
		printf("%s",db_strerror(dataBase->status));
	}
	return CurrentNew;
}

#endif