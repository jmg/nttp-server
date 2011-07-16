#include "SSLSocket.h"
#include "NNTPParser.h"
#include "Utils/Config.h"
#include "Utils/Logs.h"
#include "OpenDs/LdapService.h"
#include "OpenDs/DatabaseService.h"
#include <signal.h>

SSLSocket sslSocket;
NNTPParser nntpParser;
NNTPCommand nntpCommand;
AbstractDatabaseService databaseService;
extern char* lastNoticeId;
extern char* currentGroupName;

void Terminate(int sig)
{
	puts("Se cierra el contexto...\n");
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se cierra el contexto...", NOPARMS);
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Contexto Cerrado, Bye", NOPARMS);
	puts("Contexto Cerrado, Bye \n");
	LogFileLogEndSession();
	exit(1);
}

void onBrokenPipe(int sig) {
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "El cliente se desconecta sin aviso.", NOPARMS);
	puts("El cliente se desconecta sin aviso. \n");
	SSLSocketCloseSession(sslSocket);
	server();
}

void server() {

	for(;;) {

		int result;
		result = SSLSocketConnect(sslSocket);

		if (result == 1) {
			for(;;)
			{
				nntpParser = NewNNTPParser();
				nntpParser = NNTPParserLoadRawCommand(nntpParser, SSLSocketRead(sslSocket));
				if(NNTPParserErrors(nntpParser))
				{
					SSLSocketWrite(sslSocket, NNTPParserTooLongLineMessage(nntpParser));
					FreeNNTPParser(nntpParser);
					continue;
				}
				nntpCommand = NNTPParserParseCommand(nntpParser);
				SSLSocketWrite(sslSocket, NNTPCommandSetResponse(nntpCommand, databaseService));
				if(NNTPCommandIsQuitCommand(nntpCommand))
					break;
				FreeNNTPCommand(nntpCommand);
				FreeNNTPParser(nntpParser);
			}

			FreeNNTPCommand(nntpCommand);
			FreeNNTPParser(nntpParser);


			if(lastNoticeId)
				free(lastNoticeId);
			if(currentGroupName)
				free(currentGroupName);
		}

		SSLSocketCloseSession(sslSocket);

	}
}

int main(int argc, char *argv[])
{
	LogFileLogInitSession();
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se Inicio el programa", NOPARMS);
	if (argc != 2)
	{
		puts("No se pasaron los argumentos adecuados");
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se pasaron los argumentos adecuados", NOPARMS);
		LogFileLogEndSession();
		exit(1);
	}

	char* ldap;
	char* port;
	char* PathToCertificate;
	char* PathToKeys;

	XmlFile xmlFile = LoadConfig(argv[1]);
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se Cargo el Siguiente Archivo de Config: ", argv[1]);

	port = XmlFileGetParam(xmlFile, "Port");
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se escucha por el puerto: ", port);

	ldap = XmlFileGetParam(xmlFile, "UrlOpenDsServer");
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "El servidor open DS se encuentra en : ", ldap);

	PathToCertificate = XmlFileGetParam(xmlFile, "CertificateFile");
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "El path al certificado ssl es : ", PathToCertificate);

	PathToKeys = XmlFileGetParam(xmlFile, "KeyFile");
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "El path a las keys es: ", PathToKeys);

	FreeXmlFile(xmlFile);

	databaseService = NewDatabaseService(ldap, NULL);

	sslSocket = NewSSLSocket();
    SSLSocketInitCtx(sslSocket);
	SSLSocketLoadCertificates(sslSocket, PathToCertificate, PathToKeys);
	SSLSocketOpen(sslSocket, port);


	signal(SIGTERM, Terminate);
	signal(SIGINT, Terminate);

	signal(SIGPIPE, onBrokenPipe);

	server();

	SSLSocketClose(sslSocket);
	FreeSSLSocket(sslSocket);


	free(ldap);
	free(port);
	free(PathToCertificate);
	free(PathToKeys);

	LogFileLogEndSession();

	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se baja el servidor. ", PathToKeys);
	puts("Se baja el servidor.\n ");

    return 0;
}

