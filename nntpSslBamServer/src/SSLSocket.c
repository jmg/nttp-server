
#include "SSLSocket.h"

struct SSLSocketClass
{
    SSL_CTX *ctx;
    SSL *ssl;

    int sd;
    int sd_client;
    struct sockaddr_in addr;
    struct sockaddr_in addr_client;
	char* portnum;
};

SSLSocket NewSSLSocket(void)
{
    SSLSocket sslSocket = (SSLSocket) malloc(sizeof(struct SSLSocketClass));
    return sslSocket;
}

void FreeSSLSocket(SSLSocket sslSocket)
{
    free(sslSocket);
}

void SSLSocketOpen(SSLSocket sslSocket, char* port)
{
	sslSocket->portnum = MemReserveForChar(strlen(port));
	strcpy(sslSocket->portnum, port);
	sslSocket->sd = socket(PF_INET, SOCK_STREAM, 0);
	bzero(&sslSocket->addr, sizeof(sslSocket->addr));

    sslSocket->addr.sin_family = AF_INET;
	sslSocket->addr.sin_port = htons(atoi(sslSocket->portnum));
	sslSocket->addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sslSocket->sd, (struct sockaddr*)&sslSocket->addr, sizeof(sslSocket->addr)) != 0) {
		puts("No se pudo bindear el socket");
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se pudo bindear el socket", NOPARMS);
		LogFileLogEndSession();
		abort();
	}
	if (listen(sslSocket->sd, MAXCONNECTIONS) != 0) {
		puts("No se pudo escuchar el puerto");
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se pudo escuchar el puerto", NOPARMS);
		LogFileLogEndSession();
		abort();
	}
	free(port);
}

void SSLSocketInitCtx(SSLSocket sslSocket)
{
	SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	sslSocket->ctx = SSL_CTX_new(SSLv23_server_method());
	if (sslSocket->ctx == NULL)
	{
		ERR_print_errors_fp(stderr);
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, stderr, NOPARMS);
		LogFileLogEndSession();
		abort();
	}
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se inicializo el contexto de Open SSL", NOPARMS);
}

void SSLSocketLoadCertificates(SSLSocket sslSocket, char* CertFile, char* KeyFile)
{
	if (SSL_CTX_use_certificate_file(sslSocket->ctx, CertFile, SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, stderr, NOPARMS);
		LogFileLogEndSession();
		abort();
	}
	/* set the private key from KeyFile (may be the same as CertFile) */
	while(SSL_CTX_use_PrivateKey_file(sslSocket->ctx, KeyFile, SSL_FILETYPE_PEM) <= 0) {
		puts("Clave invalida");
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Clave invalida.", NOPARMS);
		LogFileLogEndSession();
		abort();
	}
	/* verify private key */
	if (!SSL_CTX_check_private_key(sslSocket->ctx)) {
		fprintf(stderr, "La clave privada no matchea con el certificado publico\n");
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, stderr, NOPARMS);
		LogFileLogEndSession();
		abort();
	}
}


void SSLSocketShowCerts(SSLSocket sslSocket)
{
	X509 *cert;
	char *line;

	cert = SSL_get_peer_certificate(sslSocket->ssl);
	if (cert != NULL)
	{
		puts("Server certificates:\n");
		line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
		printf("Subject: %s\n", line);
		free(line);
		line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
		printf("Issuer: %s\n", line);
		free(line);
		X509_free(cert);
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Conexion Con certificados", NOPARMS);
	}
	else
	{
		puts("Conexion Sin certificados.\n");
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Conexion Sin certificados", NOPARMS);
	}
}

int SSLSocketConnect(SSLSocket sslSocket)
{
    unsigned int len = sizeof(sslSocket->addr);

    puts("Esperando Conexion del cliente... \n");
    sslSocket->sd_client = accept(sslSocket->sd, (struct sockaddr*)&sslSocket->addr_client, &len);
    printf("Recibida conexion de: %s:%d\n", inet_ntoa(sslSocket->addr_client.sin_addr), ntohs(sslSocket->addr_client.sin_port));
    LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Recibida conexion de: ", inet_ntoa(sslSocket->addr_client.sin_addr));
    char * buffer = calloc(10, sizeof(int));
    sprintf(buffer, "%d", ntohs(sslSocket->addr_client.sin_port));
    LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Usando el puerto: ", buffer);
    free(buffer);
    char buf[256];

    sslSocket->ssl = SSL_new(sslSocket->ctx);
    SSL_set_fd(sslSocket->ssl, sslSocket->sd_client);

    if (SSL_accept(sslSocket->ssl) == -1)
    {
		ERR_print_errors_fp(stderr);
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, stderr, NOPARMS);
		return 0;
    }
	else
	{
		SSLSocketShowCerts(sslSocket);
		return 1;
	}
}

int isFinishOfMessage(char* buffer)
{
	return (buffer[8] == '\r' && buffer[9] == '\n');
}

char* SSLSocketRead(SSLSocket sslSocket)
{
	int i;
    int received;
    char* buffer = (char*) calloc(BUFFER, sizeof(char));
    char* rc = (char*) calloc(10, sizeof(char));

    for (i=1;;i++)
    {
        received = SSL_read(sslSocket->ssl, buffer, BUFFER);

        if (received > 0)
        {
        	rc = (char*) (realloc(rc, i * (BUFFER+1) * sizeof(char)));
            concatString(rc, buffer);
        }
        if (received < BUFFER || isFinishOfMessage(buffer))
            break;
        memset(buffer, '\0', BUFFER);
    }

    free(buffer);
    LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se leyó del Socket", NOPARMS);
	return rc;
}

void SSLSocketWrite(SSLSocket sslSocket, char* response)
{
    SSL_write(sslSocket->ssl, response, strlen(response));
    LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se ecribió en el Socket", NOPARMS);
}

void SSLSocketCloseSession(SSLSocket sslSocket)
{
    LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se cierra sesion", NOPARMS);
	puts("Se cierra sesion.\n");
    close(sslSocket->sd_client);
}

void SSLSocketClose(SSLSocket sslSocket)
{
    close(sslSocket->sd);
	SSL_free(sslSocket->ssl);
    LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se cerro la conexión del socket", NOPARMS);
    SSL_CTX_free(sslSocket->ctx);
}

