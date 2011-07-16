#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "socket.cpp"
#include "Console.cpp"
#include "MsQeue.cpp"
#include "XmlParser.cpp"
#include "News.cpp"
#include "IpcRpcParser.cpp"
#include "Console.cpp"
#include "ClientConnection.cpp"
#include "Utils/Configuration.h"
#include "Utils/Logs.h"
#include <signal.h>

ServerSocket serverSocket;
MsQeue NewsQeue;
IPC_RPCParser ipc_rpcParser;
IPC_RPC ipc_rpc;

#define IPC_RPCOK "00"

using namespace std;

void Terminate(int signum)
{
	puts("Connection Closed, Bye!");	
	exit(1);
}

unsigned __stdcall OnConnect(void* socket)
{

	ClientConnection clientConnection((SOCKET)socket);

	/*recepcion del handshake del publicador*/
	string str = clientConnection.read();
	if(!ipc_rpcParser.isValid(str))
	{
		clientConnection.close();
		return 0;
	}

	ipc_rpc = ipc_rpcParser.ParseMesage(str);
	if(!ipc_rpcParser.AutenticateHandShake(ipc_rpc))
	{
		clientConnection.close();
		return 0;
	}

	Console::WriteLine(str);
	puts("Se autentico conexion, se procede a contestar...");
	clientConnection.write(IPC_RPCOK);
	
	puts("Se espera el contenido a procesar...");
	str = clientConnection.read();
	if (str != "") {
		ipc_rpc = ipc_rpcParser.ParseMesage(str);
		NewsQeue.Insert((BSTR)ipc_rpc.message.c_str());
		puts("Se proceso noticia a la cola...");
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Se proceso noticia a la cola...", NOPARMS);
	}

	clientConnection.close();
	return 0;

/*	ClientConnection clientConnection((SOCKET)socket);

	string str = clientConnection.read();
	if (!ipc_rpcParser.isValid(str))
	{
		clientConnection.close();
		return 0;
	}
	if (str != "") {
		ipc_rpc = ipc_rpcParser.ParseMesage(str);
		NewsQeue.Insert((BSTR)ipc_rpc.message.c_str());
	}

	clientConnection.close();
	return 0;*/
}

int main(int argc, char* argv[])
{
	LogFileLogInitSession();
	if(argc != 2)
	{
		puts("No se pasaron los parametros adecuados");
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se pasaron los argumentos adecuados", NOPARMS);
		LogFileLogEndSession();
		exit(1);
	}
	signal(SIGTERM, Terminate);
	signal(SIGINT, Terminate);

	XmlFile xmlFile = LoadConfig(argv[1]);
	if (!xmlFile)
	{
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se encotraro el archivo de configuracion", NOPARMS);
		puts("No se encotraro el archivo de configuracion");
		LogFileLogEndSession();
		exit(1);
	}
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se cargo el siguiente archivo de configuracion: ", argv[1]);
	char* Port = XmlFileGetParam(xmlFile, "Port");
	if (!Port)
	{
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se encotraron los parametros correctos", NOPARMS);
		puts("No se encotraron los parametros correctos");
		LogFileLogEndSession();
		exit(1);
	}
	FreeXmlFile(xmlFile);
	int port = atoi(Port);
	if (!port)
	{
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "El puerto es incorrecto", NOPARMS);
		puts("El puerto es incorrecto");
		LogFileLogEndSession();
		exit(1);
	}

	Console::WriteLine("Se inicio el servidor de noticias de The Open Source Team");

	ServerSocket serverSocket;
	serverSocket.Listen(port, OnConnect);
	LogFileLogEndSession();
	MemFree(Port);

	return 0;
}

