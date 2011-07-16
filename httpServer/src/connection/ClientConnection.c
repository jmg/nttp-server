#include "ClientConnection.h"
# define BUFFER 256

struct ConcreteClientConnection {
	int socketDescriptor;
};

AbstractClientConnection NewClientConnection(int socketDescriptor) {

	AbstractClientConnection clientConnection;
	if ((clientConnection = malloc(sizeof(struct ConcreteClientConnection)))) {
		clientConnection->socketDescriptor = socketDescriptor;
	}
	return clientConnection;
}

int writeConnection(AbstractClientConnection clientConnection, char * message) {
	int len = strlen(message);
	int aux = send(clientConnection->socketDescriptor, message, len, 0);
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se escribe en socket: ", message);
	return aux;
}

char* readConnection(AbstractClientConnection clientConnection) {
	int i;
    int received;
    char* buffer = (char*) calloc(BUFFER, sizeof(char));
    char* rc = (char*) calloc(BUFFER, sizeof(char));

    for (i=1;;i++)
    {
		received = recv(clientConnection->socketDescriptor, buffer, BUFFER, 0);

        if (received > 0)
        {

            char * oldrc = rc;
            int size = ( (strlen(rc)) * sizeof(char))  + received + 1;

            rc = (char*) (realloc(rc, size));

        	if (rc != oldrc) {
        		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "ClientConnection - Se realoco el buffer en otro puntero, se hace un free en el viejo. ", NOPARMS);
        	} else if (rc == NULL) {
        		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "ClientConnection - No hay memoria para realocar el buffer. ", NOPARMS);
        	}

            strncat(rc, buffer, received);


        }


        if (received < BUFFER) {
            break;
        }

        memset(buffer, '\0', BUFFER);
    }
    free(buffer);
    LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se leyó del Socket", NOPARMS);
	return rc;

}

void ClientConnectionFree(AbstractClientConnection clientConnection) {
	close(clientConnection->socketDescriptor);
	free(clientConnection);
}

