#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "../nntpServer/XmlParser.cpp"
#include "../nntpServer/MsQeue.cpp"
#include "../nntpServer/Console.cpp"
#include "../nntpServer/Utils/Configuration.cpp"
#include "../nntpServer/Utils/Logs.cpp"
#include "OpenDs/DatabaseService.cpp"
#include <signal.h>

MsQeue NewsQeue;
XmlFile xmlFile;
BSTR XmlNew;
XmlParser xmlParser;

void Terminate(int signum)
{
	if (xmlParser.isValid(XmlNew))
		NewsQeue.Insert(XmlNew);	
	puts("Connection Closed, Bye!");	
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Connection Closed, Bye!", NOPARMS);
	LogFileLogEndSession();
	exit(1);
}

void LdapFails()
{
	if (xmlParser.isValid(XmlNew))
		NewsQeue.Insert(XmlNew);	
	puts("No se pudo conectar con Ldap. Se continua intentando...");
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se pudo conectar con ldap. Se continua intentando...", NOPARMS);
}

void NewsSaver(void)
{
	char* timeStr = XmlFileGetParam(xmlFile, "Time");
	if (!timeStr)
	{
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se encontraron los parametros correctos", NOPARMS);
		puts("No se encontraron los parametros correctos");
		LogFileLogEndSession();
		exit(1);
	}
	char* ldapServer = XmlFileGetParam(xmlFile, "ldapServer");
	if (!ldapServer)
	{
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se encontraron los parametros correctos", NOPARMS);
		puts("No se encontraron los parametros correctos");
		LogFileLogEndSession();
		exit(1);
	}
	int timing = atoi(timeStr);
	if (!timing)
	{
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se encontraron los parametros correctos", NOPARMS);
		puts("No se encontraron los parametros correctos");
		LogFileLogEndSession();
		exit(1);
	}
	else if (timing <= 0)
	{
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "El tiempo de delay no puede ser negativo o cero", NOPARMS);
		puts("El tiempo de delay no puede ser negativo");
		LogFileLogEndSession();
		exit(1);
	}
	DatabaseService databaseService(ldapServer);
	FreeXmlFile(xmlFile);	

	for(;;)
	{
		XmlNew = NewsQeue.Get();
		if (!XmlNew)
		{
			Sleep(timing);
			continue;
		}
		New CurrentNew;
		if (xmlParser.isValid(XmlNew))
			CurrentNew = xmlParser.ParseXml(XmlNew);
		else
			continue;
		
		if (databaseService.uploadNotice(CurrentNew) != 0)
			LdapFails();
		else
			Console::WriteLine("Se guardo una noticia en Ldap");
		
		Sleep(timing);
	}
	MemFree(timeStr);
	MemFree(ldapServer);
}

int main(int argc, char* argv[])
{
	LogFileLogInitSession();
	if(argc != 2)
	{
		puts("No se pasaron los parametros adecuados");
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se pasaron los argumentos adecuados", NOPARMS);
		exit(1);
	}	
	signal(SIGTERM, Terminate);
	signal(SIGINT, Terminate);

	xmlFile = LoadConfig(argv[1]);	
	if (!xmlFile)
	{
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se encontro el archivo dde configuracion", NOPARMS);
		puts("No se encontro el archivo dde configuracion");
		LogFileLogEndSession();
		exit(1);
	}
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se cargo el archivo de configuracion: ", argv[1]);		
	Console::WriteLine("Se inicio el servidor de acceso a datos de The Open Source Team");
	NewsSaver();
	LogFileLogEndSession();
}

