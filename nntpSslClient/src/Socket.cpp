#ifndef SOCKET_CPP
#define SOCKET_CPP
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#include "StrMng.cpp"
#include "Console.cpp"

class Socket {

protected:
	int sockfd;
	struct sockaddr_in address;
	struct hostent *server;
	static const int BUFFER = 10;

	virtual int sendMessage(char* buffer, int len) {
		return send(sockfd, buffer, len, 0);
	}

	virtual int receiveMessage(char* buffer, int len) {
		return recv(sockfd, buffer, len, 0);
	}

public:

	Socket(string host, int port) {
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) {
			perror("Hubo un error creando el socket cliente.");
			exit(1);
		}

		address.sin_family = AF_INET;
		address.sin_port = htons(port);
		server = gethostbyname(StrMng::ToCharArray(host));
		address.sin_addr = *((struct in_addr *) server->h_addr);
		memset(&(address.sin_zero), '\0', BUFFER);

		int result = connect(sockfd, (struct sockaddr *) &address,
				sizeof(address));
		if (result == -1) {
			perror("Hubo un error conectando el socket cliente.");
			exit(1);
		}
   	}

	virtual string read() {

		int i;
	    int received;
	    char* buffer = (char*) calloc(BUFFER, sizeof(char));
	    char* rc = (char*) calloc(10, sizeof(char));

	    puts("Log: El socket espera la lectura de un mensaje \n");
	    for (i=1;;i++)
	    {
	    	received = receiveMessage(rc, BUFFER);
	    	printf("Log: El socket recibe los bits: %i \n", received);

	        if (received > 0)
	        {
	        	rc = (char*) (realloc(rc, i * (BUFFER+1) * sizeof(char)));
	            StrMng::concatString(rc, buffer);
	        } else {
	        	break;
	        }

	        if (received < BUFFER)
	            break;
	        memset(buffer, '\0', BUFFER);
	    }

	    free(buffer);
	    printf("Log: El socket lee el mensaje: %s \n", rc);
		return rc;

		return StrMng::getString(rc);
	}

	virtual int write(string cadena) {
		char* tempWrite = StrMng::ToCharArray(cadena);
		int len = cadena.length();
		int aux = sendMessage(tempWrite, len);
		free(tempWrite);
		return aux;
	}

};
#endif
