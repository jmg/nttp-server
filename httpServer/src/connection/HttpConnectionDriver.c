#include "HttpConnectionDriver.h"

#define ERROR_404 404
#define ERROR_500 500

char NOTICE_PAGE[] = "NoticePage.html?";
char GROUP_PAGE[] = "GroupPage.html?";
char INDEX_PAGE[] = "Index.html";
char NOTICE_PARAM[] = "notice=";
char GROUP_PARAM[] = "group=";



void HttpConnectionDriverRedirectToPageNotice(
		AbstractHttpConnectionDriver httpConnectionDriver, char * noticeId);
void HttpConnectionDriverRedirectToIndex(
		AbstractHttpConnectionDriver httpDriver);
void HttpConnectionDriverRedirectToPageOfGroupName(
		AbstractHttpConnectionDriver httpConnectionDriver, char * groupName);
void redirectToErrorPage(AbstractClientConnection clientConnection, int errorCode);

struct ConcreteHttpConnectionDriver {
	char * baseUrl;
	AbstractClientConnection clientConnection;
	AbstractDatabaseService abstractDatabaseService;
};

AbstractHttpConnectionDriver NewHttpConnectionDriver(char * baseUrl,
		AbstractDatabaseService abstractDatabaseService,
		AbstractClientConnection clientConnection) {

	AbstractHttpConnectionDriver httpConnectionDriver;
	if ((httpConnectionDriver = malloc(
			sizeof(struct ConcreteHttpConnectionDriver)))) {
		httpConnectionDriver->clientConnection = clientConnection;
		httpConnectionDriver->abstractDatabaseService = abstractDatabaseService;
		httpConnectionDriver->baseUrl = baseUrl;
	}

	return httpConnectionDriver;
}

void start(AbstractHttpConnectionDriver httpDriver) {
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se leera el REQUEST de la conexion entrante. ", NOPARMS);

	char * received = readConnection(httpDriver->clientConnection);

	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se recibe REQUEST: ", received);

	AbstractUrlParser urlParser = NewUrlParser(received);
	free(received);

	int parseResult;
	parseResult = UrlParserGetParseResult(urlParser);

	if (parseResult == URLPARSER_SUCCESS) {

		if (UrlParserIsIndexPage(urlParser)) {

			LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se redirije a la HOME ", NOPARMS);
			HttpConnectionDriverRedirectToIndex(httpDriver);

		} else if (UrlParserIsGroupPage(urlParser)) {

			LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se redirije a la GROUP PAGE del grupo: ", UrlParserGetQueryString(urlParser));
			HttpConnectionDriverRedirectToPageOfGroupName(httpDriver,
					UrlParserGetQueryString(urlParser));

		} else if (UrlParserIsNoticePage(urlParser)) {

			LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se redirije a la NOTICE PAGE con id: ", UrlParserGetQueryString(urlParser));
			HttpConnectionDriverRedirectToPageNotice(httpDriver,
					UrlParserGetQueryString(urlParser));

		}

	} else	if ( parseResult == URLPARSER_ERROR_INVALID_PATH || parseResult == URLPARSER_ERROR_INVALID_PARAM) {
		redirectToErrorPage(httpDriver->clientConnection, ERROR_404);
	}

	UrlParserFree(urlParser);

	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se termina el ciclo de RESPONSE ", NOPARMS);

	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se libera memoria del ciclo de RESPONSE ", NOPARMS);
	HttpConnectionDriverFree(httpDriver);
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se muere el thread ", NOPARMS);
}

void HttpConnectionDriverFree(
		AbstractHttpConnectionDriver abstractHttpConnectionDriver) {
	ClientConnectionFree(abstractHttpConnectionDriver->clientConnection);
	//No lo libero porq es una variable q es utilizada en contexto.
	//free(abstractHttpConnectionDriver->baseUrl);
	free(abstractHttpConnectionDriver);
}

char * getHtmlLink(char * title, char * url) {
	char * part1 = "<div style='text-align:center;padding:5px;border:3px solid;margin:10px;'><a href='";
	char * lastPart = "'>";
	char * endTag = "</a></div>\n\0";
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se prepara link para el title: ", title);

	int size = strlen(part1) + strlen(lastPart) + strlen(title)
			+ strlen(endTag) + strlen(url);
	char * link = calloc(size + 10, sizeof(char));
	strcat(link, part1);
	strcat(link, url);
	strcat(link, lastPart);
	strcat(link, title);
	strcat(link, endTag);

	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se termina de preparar el link para el title: ", title);

	return link;
}

char * getNoticePageUrl(AbstractHttpConnectionDriver httpConnectionDriver,
		AbstractNotice notice) {
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se prepara url para notice con id: ", getId(notice));

	int size;
	size = strlen(httpConnectionDriver->baseUrl) + strlen(NOTICE_PARAM)
			+ strlen(getId(notice)) + strlen(NOTICE_PAGE);
	char * url = calloc(size + 10, sizeof(char));
	strcat(url, httpConnectionDriver->baseUrl);
	strcat(url, NOTICE_PAGE);
	strcat(url, NOTICE_PARAM);
	strcat(url, getId(notice));

	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se termina de preparar la url para notice con id: ", getId(notice));

	return url;
}

char * getGroupPageUrl(AbstractHttpConnectionDriver httpConnectionDriver,
		char * groupName) {
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se prepara url para grupo con nombre: ", groupName);

	int size = strlen(httpConnectionDriver->baseUrl) + strlen(GROUP_PARAM)
			+ strlen(groupName) + strlen(GROUP_PAGE);
	char * url = calloc(size + 10, sizeof(char));
	strcat(url, httpConnectionDriver->baseUrl);
	strcat(url, GROUP_PAGE);
	strcat(url, GROUP_PARAM);
	strcat(url, groupName);

	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se termina de preparar url para grupo con nombre: ", groupName);

	return url;
}


void redirectToErrorPage(AbstractClientConnection clientConnection, int errorCode) {

	if (errorCode == ERROR_404) {
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se redirije a la pagina de error con Codigo de error: 404", NOPARMS);
		writeConnection(clientConnection,
				"HTTP/1.1 404 Not Found\n\n<b>ERROR</b>: File was not found\n\0");
	} else if (errorCode == ERROR_500) {
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se redirije a la pagina de error con Codigo de error: 500", NOPARMS);
		writeConnection(clientConnection,
				"HTTP/1.1 500 Internal Server Error\n\n<b>ERROR</b>: El servicio se encuentra temporalmente no disponible.\n\0");
	} else {
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se redirijio a una pagina con codigo de error no valido ", NOPARMS);
		exit(1);
	}
}

char * decoratePage(char * content) {
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se prepara el decorado con los headers ", NOPARMS);

	char
			* pageHeader =
					"<HTML><HEAD><TITLE>The Open Source Team Notice Server</TITLE></HEAD><BODY><h2 style='text-align:center;'>The Open Source Team Notice Server</h2><br/>";
	char * pageFooter = "</BODY></HTML>\r\n";

	char * part1 = "HTTP/1.0 200 OK\r\nServer: Mi Servidor\r\nContent-Length: ";
	char * part2 = " \r\nContent-Type: text/html\r\nConnection: Close\r\n\r\n";

	char * contentLenght = calloc(10, sizeof(int));
	int contentSize = ((strlen(content) + strlen(pageHeader) + strlen(
			pageFooter)) * sizeof(char));
	sprintf(contentLenght, "%i", contentSize);

	int size = strlen(pageHeader) + strlen(pageFooter) + strlen(part1)
			+ strlen(contentLenght) + strlen(content) + strlen(part2);
	char * response = calloc(size + 10, sizeof(char));
	strcat(response, part1);
	strcat(response, contentLenght);
	strcat(response, part2);
	strcat(response, pageHeader);
	strcat(response, content);
	strcat(response, pageFooter);

	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se termina el decorado con los headers ", NOPARMS);

	return response;
}

void HttpConnectionDriverRedirectToIndex(
		AbstractHttpConnectionDriver httpDriver) {
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Obtengo los grupos de noticias. ", NOPARMS);
	AbstractNoticeGroups noticeGroups = DatabaseServiceGetNoticesGroups(
			httpDriver->abstractDatabaseService);

	if (noticeGroups == NULL) {
		redirectToErrorPage(httpDriver->clientConnection, ERROR_500);
	} else {
		int noticeGroupsCount = NoticeGroupGetGroupsCount(noticeGroups);
		int var = 0;
		char * groupsLinks[noticeGroupsCount];
		int groupsLinksSize = 0;

		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se prepara el contenido . ", NOPARMS);
		for (; var < noticeGroupsCount; ++var) {
			char * groupName = NoticeGroupGetGroupName(noticeGroups, var);
			char * url = getGroupPageUrl(httpDriver, groupName);
			char * link = getHtmlLink(groupName, url);

			int linkSize = strlen(link) + 10;
			groupsLinks[var] = calloc(linkSize, sizeof(char));
			strcpy(groupsLinks[var], link);

			free(link);
			free(url);

			//No hacer free, es estaticaaaa
			//free(groupName);
			groupsLinksSize = groupsLinksSize + linkSize;
		}
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se termino de preparar el contenido. ", NOPARMS);

		int var2 = 0;
		char * htmlResponse = calloc(groupsLinksSize + 1000, sizeof(char));
		for (; var2 < noticeGroupsCount; ++var2) {
			strcat(htmlResponse, groupsLinks[var2]);
			free(groupsLinks[var2]);
		}

		char * decoratedResponse = decoratePage(htmlResponse);
		writeConnection(httpDriver->clientConnection, decoratedResponse);
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se contesta el request con el siguiente RESPONSE ", decoratedResponse);
		free(htmlResponse);
		free(decoratedResponse);

		NoticeGroupFree(noticeGroups);
	}
}

void HttpConnectionDriverRedirectToPageOfGroupName(
		AbstractHttpConnectionDriver httpConnectionDriver, char * groupName) {
	AbstractStack stack = DatabaseServiceGetNoticesByGroupName(
			httpConnectionDriver->abstractDatabaseService, groupName);

	if (stack == NULL) {
		redirectToErrorPage(httpConnectionDriver->clientConnection, ERROR_404);
	} else {

		int noticeQuantity = StackGetSize(stack);
		char * noticeLinks[noticeQuantity];

		int c = 0;
		int noticeLinksSizeAcum = 0;
		while (!StackIsEmpty(stack)) {
			AbstractNotice notice = StackPop(stack);

			char * url = getNoticePageUrl(httpConnectionDriver, notice);
			char * noticeLink = getHtmlLink(getHeader(notice), url);

			int noticeLinkSize = strlen(noticeLink) + 5;
			noticeLinks[c] = calloc(noticeLinkSize, sizeof(char));

			strcpy(noticeLinks[c], noticeLink);
			noticeLinksSizeAcum = noticeLinksSizeAcum + noticeLinkSize;
			free(noticeLink);
			free(url);
			NoticeFree(notice);
			c++;
		}

		int var2 = 0;

		char * titlePrefix = "<h2>Notices for '";
		char * titleSufix = "'</h2>";

		char * groupTitle = MemReserveForChar(strlen(titleSufix)
				+ strlen(groupName) + strlen(titlePrefix));
		strcat(groupTitle, titlePrefix);
		strcat(groupTitle, groupName);
		strcat(groupTitle, titleSufix);

		char * htmlResponse = calloc(noticeLinksSizeAcum + strlen(groupTitle) + 1,
				sizeof(char));

		strcat(htmlResponse, groupTitle);
		for (; var2 < noticeQuantity; ++var2) {
			strcat(htmlResponse, noticeLinks[var2]);
			free(noticeLinks[var2]);
		}

		char * decoratedResponse = decoratePage(htmlResponse);
		writeConnection(httpConnectionDriver->clientConnection, decoratedResponse);
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se contesta el request con el siguiente RESPONSE ", decoratedResponse);

		free(htmlResponse);
		free(decoratedResponse);
		StackFree(stack);
	}
}

void HttpConnectionDriverRedirectToPageNotice(
		AbstractHttpConnectionDriver httpConnectionDriver, char * noticeId) {
	AbstractNotice notice = DatabaseServiceGetNoticesById(
			httpConnectionDriver->abstractDatabaseService, noticeId);

	if (notice == NULL) {
		redirectToErrorPage(httpConnectionDriver->clientConnection, ERROR_404);
	} else {
		char * headerPrefix = "<div style='text-align:center;padding:5px;border:3px solid;margin-bottom:20px;'>";
		char * header = getHeader(notice);
		char * headerSufix = "</div>";
		char * bodyPrefix = "<div style='padding:5px;border:3px solid;margin-bottom:20px;'>";
		char * body = getBody(notice);
		char * bodySufix = "</div>\0";

		int contentLenght = strlen(headerPrefix) + strlen(header) + strlen(
				headerSufix) + strlen(bodyPrefix) + strlen(body)
				+ strlen(bodySufix);

		char * htmlResponse = calloc(contentLenght + 1, sizeof(char));

		strcat(htmlResponse, headerPrefix);
		strcat(htmlResponse, header);
		strcat(htmlResponse, headerSufix);
		strcat(htmlResponse, bodyPrefix);
		strcat(htmlResponse, body);
		strcat(htmlResponse, bodySufix);

		char * decoratedResponse = decoratePage(htmlResponse);
		writeConnection(httpConnectionDriver->clientConnection, decoratedResponse);
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se contesta el request con el siguiente RESPONSE ", decoratedResponse);

		free(htmlResponse);
		free(decoratedResponse);
		/*Lo agregue, guarda q puede romper todoooo!*/
		free(notice);
	}
}

