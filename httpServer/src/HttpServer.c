#ifndef HTTPSERVER_C
#define HTTPSERVER_C
#include <stdio.h>
#include <stdlib.h>
#include "server/Thread.h"
#include "db/MemCachedService.h"
#include "connection/HttpConnectionDriver.h"
#include "server/ServerSocket.h"
#include "db/LdapService.h"
#include "utils/Config.h"
#include "Notice.h"
#include <signal.h>

char * host;
char * port;
char * cacheServerUrl;
char * cacheServerUrl2;
char * dbServerUrl;
char * baseUrl;
char * cacheServerPort;
char * cacheServerPort2;

AbstractServerSocket serverSocket;

AbstractDatabaseService abstactDatabaseService;
AbstractLdapService abstractLdapService;

void onConnect(void * clientConnection) {
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se acepto una nueva conexion.", NOPARMS);

	AbstractHttpConnectionDriver httpDriver =
			NewHttpConnectionDriver(baseUrl, abstactDatabaseService,
					(AbstractClientConnection) clientConnection);
	start(httpDriver);
}

void initHttpServerContext(char * pathToConfigurationFile) {
/*	host = "192.168.2.197";
	port = "8012";
	cacheServerUrl = "192.168.2.197";
	cacheServerUrl2 = "192.168.2.197";
	dbServerUrl = "ldap://192.168.2.234:1389";
*/
	XmlFile xmlFile = LoadConfig(pathToConfigurationFile);
	if (!xmlFile) {
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Archivo de configuracion no encontrado en la ruta ", pathToConfigurationFile);
		exit(1);
	}

	port = XmlFileGetParam(xmlFile, "Port");
	if (!port)
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Parametro Port no encontrado", NOPARMS);

	host = XmlFileGetParam(xmlFile, "IpLocal");
	if (!host)
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Parametro IpLocal no encontrado", NOPARMS);

	cacheServerUrl = XmlFileGetParam(xmlFile, "UrlMecachedServer");
	if (!cacheServerUrl)
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Parametro UrlMecachedServer no encontrado", NOPARMS);

	cacheServerPort = XmlFileGetParam(xmlFile, "PortMecachedServer");
	if (!cacheServerPort)
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Parametro UrlMecachedServer no encontrado", NOPARMS);

	cacheServerUrl2 = XmlFileGetParam(xmlFile, "UrlMecachedServer2");
	if (!cacheServerUrl2)
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Parametro UrlMecachedServer2 no encontrado", NOPARMS);

	cacheServerPort2 = XmlFileGetParam(xmlFile, "PortMecachedServer2");
	if (!cacheServerPort2)
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Parametro UrlMecachedServer2 no encontrado", NOPARMS);

	dbServerUrl = XmlFileGetParam(xmlFile, "UrlOpenDsServer");
	if (!dbServerUrl)
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Parametro UrlOpenDsServer no encontrado", NOPARMS);

	FreeXmlFile(xmlFile);

	char * protocol = "http://";
	char * portSeparator = ":";
	char * terminator = "/";

	int baseUrlLength = strlen(protocol) + strlen(portSeparator) + strlen(
			terminator) + strlen(port) + strlen(host);
	baseUrl = calloc(baseUrlLength + 1, sizeof(char));
	strcat(baseUrl, protocol);
	strcat(baseUrl, host);
	strcat(baseUrl, portSeparator);
	strcat(baseUrl, port);
	strcat(baseUrl, terminator);

	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Inicializadas las variables de contexto", NOPARMS);
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Ldap server: ", dbServerUrl);
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Cached url: ", cacheServerUrl);
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Cached url 2: ", cacheServerUrl2);
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Base url: ", baseUrl);

}

void terminate(int s) {
	puts("Bajando servidor...");
	ServerSocketClose(serverSocket);
	puts("Servidor Terminado...");
	exit(1);
}

int main(int argc, char *argv[]) {
	LogFileLogInitSession();
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se Inicio el programa", NOPARMS);
	if (argc != 2)
	{
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se pasaron los argumentos adecuados", NOPARMS);
		LogFileLogEndSession();
		exit(1);
	}

	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se inicia el contexto recibiendo como parametro el siguiente archivo:", argv[1]);

	initHttpServerContext(argv[1]);

	signal(SIGTERM, terminate);
	signal(SIGINT, terminate);
	signal(SIGPIPE, terminate);

	abstactDatabaseService = NewDatabaseService(dbServerUrl, cacheServerUrl, cacheServerUrl2, atoi(cacheServerPort), atoi(cacheServerPort2));

	serverSocket = NewServerSocket(atoi(port));

	ServerSocketListen(serverSocket, onConnect);

	return 0;
}

#endif
