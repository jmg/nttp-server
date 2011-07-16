#include "ServerSocket.h"

struct ConcreteServerSocket {
	int port;
	struct sockaddr_in sin;
	struct sockaddr_in pin;
	int sock_descriptor;
	int temp_sock_descriptor;
	int address_size;
};

AbstractServerSocket NewServerSocket(int port) {
	AbstractServerSocket abstractSocket;
	if ((abstractSocket = malloc(sizeof(struct ConcreteServerSocket)))) {
		/* Definicion de socket */
		abstractSocket->port = port;
		abstractSocket->sock_descriptor = socket(AF_INET, SOCK_STREAM, 0);
		if (abstractSocket->sock_descriptor == -1) {
			perror("llamada a socket");
			exit(1);
		}

		/* definiendo valor de componentes de la estructura del socket */
		abstractSocket->sin.sin_family = AF_INET;
		abstractSocket->sin.sin_addr.s_addr = INADDR_ANY;
		abstractSocket->sin.sin_port = htons(abstractSocket->port);

		if (bind(abstractSocket->sock_descriptor,
				(struct sockaddr *) &(abstractSocket->sin),
				sizeof(abstractSocket->sin)) == -1) {
			perror("llamada para unir");
			exit(1);
		}

		/* Esperando conexiones */
		if (listen(abstractSocket->sock_descriptor, 20) == -1) {
			perror("llamada para escuchar");
			exit(1);
		} else {
			puts("LOG: Aceptando conexiones ... \n");
		}
	}
	return abstractSocket;
}

void ServerSocketListen(AbstractServerSocket abstractSocket, void ( onConnect)(void *)) {
	abstractSocket->address_size = sizeof abstractSocket->pin;
	while (1) {
		abstractSocket->temp_sock_descriptor = accept(
				abstractSocket->sock_descriptor,
				(struct sockaddr *) &(abstractSocket->pin),
				&(abstractSocket->address_size));

		if (abstractSocket->temp_sock_descriptor == -1) {
			perror("llamada para aceptar");
			exit(1);
		}
		printf("Conexion aceptada ... \n");
		AbstractThread threadConexion = NewThread();
		runWithParam(threadConexion, onConnect,
				(void *) NewClientConnection(abstractSocket->temp_sock_descriptor));
	}
}

void ServerSocketClose(AbstractServerSocket abstractSocket) {
	close(abstractSocket->temp_sock_descriptor);
}
