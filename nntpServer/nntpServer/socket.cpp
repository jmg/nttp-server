#ifndef Socket_CPP
#define Socket_CPP

#define DllExport __declspec(dllexport)
#include <winsock2.h>
#include <stdio.h>
#include "thread.h"
#include "Utils/Logs.h"

class ServerSocket
{
	private:
		static const int MAXSOCKETS = 5;
		WSADATA wsaData;    
		SOCKET sock;
		sockaddr_in addr;
		int NumConnections;
		int addr_size;
		Thread thread;

	public:
		ServerSocket()
		{
			if (WSAStartup(MAKEWORD(2,2), &wsaData) != NO_ERROR) 
			{
				WSACleanup();
				puts("Error at WSAStartup()\n");
				LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Error de inicializacion del socket", NOPARMS);
				LogFileLogEndSession();
				exit(1);
			}
			if (wsaData.wVersion != 0x0202)
			{ 
				WSACleanup();
				puts("error version");
				LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Error de version del socket", NOPARMS);
				LogFileLogEndSession();
				exit(1);
			}
			sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);			
		}
		void Listen(int port, unsigned (__stdcall * target)(void*))
		{
			addr.sin_family = AF_INET;      
			addr.sin_port = htons(port);   
			addr.sin_addr.s_addr = htonl(INADDR_ANY);  

			if (bind(sock, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
			{ 
				WSACleanup();  
				puts("No se pudo bindear el socket");
				LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se pudo bindear el socket", NOPARMS);
				LogFileLogEndSession();
				exit(1);
			}

			if (listen(sock, MAXSOCKETS) == SOCKET_ERROR)
			{ 
				WSACleanup();
				puts("No se pudo escuchar el puerto");
				LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se pudo escuchar el puerto", NOPARMS);
				LogFileLogEndSession();
				exit(1);
			}
			
			for (;;) 
			{
				SOCKET socketClient;
				socketClient = accept(sock, NULL, NULL); 
				if (socketClient == INVALID_SOCKET)
				{
					WSACleanup ();
					puts("Error de coneccion del cliente");
					LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Error de coneccion del cliente", NOPARMS);
				}
				else
				{
					thread = NewThread();
					ThreadRun(thread, target, socketClient);
					ThreadWaitForFinish(thread);
					ThreadDestroy(thread);
				}
			}
		}
};
#endif