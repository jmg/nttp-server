#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

#include "StrMng.h"
#include "News.h"
#include "Thread.h"
#include "DataBase.h"
#include "Socket.h"
#include "IpcRpc.h"
#include "Utils/Config.h"
#include "Utils/Logs.h"
#include <signal.h>

XmlFile xmlFile;

char* Host;
char* Port;
char* TimeStr;
char* NewsGroup;
char* DbPath;
int finish = 0;
Thread ClientSenderThread;
Thread NewsLoaderThread;

void Terminate(int signum)
{
	puts("Connection Closed, Bye!");
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Connection Closed, Bye!", NOPARMS);
	LogFileLogEndSession();
	exit(1);
}

int ShowMenu()
{
	puts("Que desea hacer?");
	puts("-----------------");
	puts("1. Ingresar una noticia");
	puts("2. Salir");
	
	return getchar();
}

void NewsLoader(void)
{
	int action = 0;
	while(action != 2)
	{
		New PublisherNew;
		action = 0;
		while(action == 0)
		{
			switch (ShowMenu())
			{
				case '2':
					action = 2;
					break;
				case '1':
					action = 1;
					break;
				default:
					action = 0;
					break;
			}
			fflush(stdin);
		}
		fflush(stdin);

		switch(action)
		{
			case 1:				
				PublisherNew.IdNew = Random();
				strcpy(PublisherNew.NewsGroup, NewsGroup);

				puts("Ingrese el Encabezado de la Noticia");
				GetString(PublisherNew.Head);
				while(isEmpty(PublisherNew.Head))
				{
					puts("No se permiten campos vacios o con caracteres invalidos.");
					puts("Ingrese el Encabezado de la Noticia");
					GetString(PublisherNew.Head);
				}
				puts("Ingrese el Cuerpo de la Noticia");
				GetString(PublisherNew.Body);
				while(isEmpty(PublisherNew.Body))
				{
					puts("No se permiten campos vacios o con caracteres invalidos.");
					puts("Ingrese el Cuerpo de la Noticia");
					GetString(PublisherNew.Body);
				}
								
				SaveInDataBase(PublisherNew);
				break;
			case 2:
				break;
		}
	}
	MemFree(NewsGroup);
	return 0;
}

void ClientSender(void)
{
	XmlNew xmlNew;
	Socket Sock;
	IPC_RPC ipc_rpc;	
	int result;			
	char * response;
	Sock = NewSocket();		

	result = SocketConnect(Sock, Host, atoi(Port));

	if (result != SOCKET_ERROR) {		
		xmlNew = NewXmlNew();


		xmlNew->CurrentNew = ScanDataBase();

		if (xmlNew->CurrentNew.IdNew != NOUNEXPORTEDNEWS && 
			xmlNew->CurrentNew.IdNew != DB_NOTFOUND) {

			LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se prepara el envio del HandShake", NOPARMS);

			/*handshake del publicador*/
			ipc_rpc = IPC_RPCPrepareHandShake();
			SocketWrite(Sock, IPC_RPCSerialize(ipc_rpc));/*BYJUGKBQPDCWWRDB00190Publisher-HandShake IPC_RPCSerialize(ipc_rpc));*/
			FreeIPC_RPC(ipc_rpc);

			LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se envio del HandShake, se espera respuesta...", NOPARMS);			
			response = SocketRead(Sock);
			if(strcmp(response,IPCRPCOK))
			{
				puts("Error en el envio del HandShake");
				LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Error en el envio del HandShake", NOPARMS);
				MemFree(response);

			} else {
				LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Recibio Ok. Procede con la carga...", NOPARMS);	
				
				XmlNewPrepareXML(xmlNew);
				ipc_rpc = IPC_RPCPrepareMessage(xmlNew->XmlStructure);
				SocketWrite(Sock, IPC_RPCSerialize(ipc_rpc));		

				UpdateDataBase(xmlNew->CurrentNew);
				puts("Se proceso una noticia.");
				LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se proceso una noticia ", NOPARMS);			
				
				FreeIPC_RPC(ipc_rpc);
				MemFree(response);

			}
		}
		
		FreeXmlNew(xmlNew);		
	
		close(Sock);
	}
	FreeSocket(Sock);
	return 0;
}

int main(int argc, char* argv[])
{
	int timing;
	LogFileLogInitSession();
	
	if(argc != 2)
	{
		puts("No se pasaron los argumentos adecuados");
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se pasaron los argumentos adecuados", NOPARMS);
		exit(1);
	}

	signal(SIGTERM, Terminate);
	signal(SIGINT, Terminate);

	xmlFile = NewXmlFile(argv[1]);
	if (!xmlFile)
	{
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se encotraro el archivo de configuracion", NOPARMS);
		puts("No se encotraro el archivo de configuracion");
		LogFileLogEndSession();
		exit(1);
	}
	
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se cargo el siguiente archivo de configuracion: ", argv[1]);

	Host = XmlFileGetParam(xmlFile, "Host");
	if(!Host)
	{
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se encotraron los parametros correctos", NOPARMS);
		puts("No se encotraron los parametros correctos");
		LogFileLogEndSession();
		exit(1);
	}
	Port = XmlFileGetParam(xmlFile, "Port");
	if(!Port)
	{
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se encotraron los parametros correctos", NOPARMS);
		puts("No se encotraron los parametros correctos");
		LogFileLogEndSession();
		exit(1);
	}
	TimeStr = XmlFileGetParam(xmlFile, "Time");
	if(!TimeStr)
	{
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se encotraron los parametros correctos", NOPARMS);
		puts("No se encotraron los parametros correctos");
		LogFileLogEndSession();
		exit(1);
	}
	else if (atoi(TimeStr) <= 0)
	{
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "El tiempo de delay no puede ser negativo", NOPARMS);
		puts("El tiempo de delay no puede ser negativo");
		LogFileLogEndSession();
		exit(1);
	}
	DbPath = XmlFileGetParam(xmlFile, "DbPath");
	if (!DbPath)
	{
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se encotraron los parametros correctos", NOPARMS);
		puts("No se encotraron los parametros correctos");
		LogFileLogEndSession();
		exit(1);
	}
	NewsGroup = XmlFileGetParam(xmlFile, "NewsGroup");	
	if (!NewsGroup)
	{
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se encotraron los parametros correctos", NOPARMS);
		puts("No se encotraron los parametros correctos");
		LogFileLogEndSession();
		exit(1);
	}
	timing = atoi(TimeStr);	
	if (!timing)
	{
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "El tiempo es inválido", NOPARMS);
		puts("El tiempo es inválido");
		LogFileLogEndSession();
		exit(1);
	}

	CreateOrOpenDataBase(DbPath);
	MemFree(DbPath);
	FreeXmlFile(xmlFile);

    NewsLoaderThread = NewThread();
	ThreadRun(NewsLoaderThread, NewsLoader);	

	
	while(ThreadIsAlive(NewsLoaderThread))
	{
		ClientSenderThread = NewThread();
		ThreadRun(ClientSenderThread, ClientSender);
		ThreadWaitForFinish(ClientSenderThread);
		ThreadDestroy(ClientSenderThread);
		Sleep(timing);
	}	
	CloseDataBase();

	MemFree(Host);
	MemFree(Port);
	MemFree(TimeStr);
	LogFileLogEndSession();

    return 0;
} 
