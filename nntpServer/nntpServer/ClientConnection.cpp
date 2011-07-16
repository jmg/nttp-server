#ifndef CLIENTCONNECTION_CPP
#define CLIENTCONNECTION_CPP

#include <iostream>
#include <string>
#include <signal.h>
#include "StrMng.cpp"

#define BUFFER 10

using namespace std;

class ClientConnection
{
	private:
		SOCKET socket;

	protected:

		virtual int sendMessage(char* buffer, int len) {
			return send(socket, buffer, len, 0);
		}

		virtual int receiveMessage(char* buffer, int len) {
			return recv(socket, buffer, len, 0);
		}

	public:

		ClientConnection(SOCKET socket2) {
			socket = socket2;
		}

		string read() {

			int i;
			int received;
			char* buffer = (char*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, BUFFER+1 * sizeof(char));
			char* rc = (char*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, BUFFER+1 * sizeof(char));

			for (i=1;;i++)
			{
	    		received = receiveMessage(buffer, BUFFER);
				if (received < 0)
				{
					return "";
				}

				if (received > 0)
				{
					rc = (char*) (HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, rc, i * (BUFFER+1) * sizeof(char)));
					strcat(rc, buffer);
				} else {
	        		break;
				}

				if (received < BUFFER)
					break;
				memset(buffer, '\0', BUFFER);
			}

			HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, buffer);
			return rc;

			return StrMng::getString(rc);
		}

		int write(string cadena) {
			char* tempWrite = StrMng::ToCharArray(cadena);
			int len = cadena.length();
			int aux = sendMessage(tempWrite, len);
			HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, tempWrite);
			return aux;
		}

		void close() {
			closesocket(socket);
		}
};

#endif