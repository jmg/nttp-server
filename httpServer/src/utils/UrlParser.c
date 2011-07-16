#include "UrlParser.h"

#define NOTICE_PAGE1 "NoticePage.html"
#define GROUP_PAGE1 "GroupPage.html"
#define INDEX_PAGE1 "Index.html"
#define NOTICE_PARAM1 "notice"
#define GROUP_PARAM1 "group"
#define URLPARSER_VALID_REQUEST 10

struct ConcreteUrlParser {
	char* url;
	char* path;
	char* type;
	char* queryString;
	int result;

};

int _HaveValidPath(AbstractUrlParser urlParser) {
	if (!strcmp(NOTICE_PAGE1, urlParser->path) || !strcmp(GROUP_PAGE1,
			urlParser->path) || !strcmp(INDEX_PAGE1, urlParser->path))
		return 1;
	return 0;
}

int _HaveValidParam(AbstractUrlParser urlParser) {
	if (!strcmp(NOTICE_PARAM1, urlParser->type) || !strcmp(GROUP_PARAM1,
			urlParser->type) || strlen(urlParser->type) == 0 && !strcmp(
			INDEX_PAGE1, urlParser->path))
		return 1;
	return 0;
}

int isHtmlSpace(char * url, int *i) {
	if (url[*i] == '%' && url[*i + 1] && url[*i + 1] == '2' && url[*i + 2] && url[*i + 2] == '0') {
		*i += 2;
		return 1;
	}
	return 0;
}

void validateRequest(AbstractUrlParser urlParser) {
	char * protocolSuffix = "GET /";
	if(strncmp(urlParser->url, protocolSuffix, strlen(protocolSuffix)) != 0) {
		puts("\n\nLOG: Error parseando el REQUEST.\n");
		urlParser->result = URLPARSER_ERROR_INVALID_REQUEST;
		return;
	}

	char * subString;
	char * subString2;

	subString= strstr(urlParser->url, "HTTP/1.1");

	if (subString == NULL || (subString != NULL && strlen(subString) < 1)) {
		puts("\n\nLOG: Error parseando el REQUEST.\n");

		urlParser->result = URLPARSER_ERROR_INVALID_REQUEST;
		return;
	} else {
		subString2 = strstr(subString, "Host:");

		if (subString2 == NULL || (subString2 != NULL && strlen(subString2) < 1)) {
			puts("\n\nLOG: Error parseando el REQUEST.\n");

			urlParser->result = URLPARSER_ERROR_INVALID_REQUEST;
			return;
		}
	}
	urlParser->result = URLPARSER_VALID_REQUEST;
	puts("\n\nLOG: El REQUEST es valido.\n");
}

void _ParseUrl(AbstractUrlParser urlParser) {
	validateRequest(urlParser);
	if (urlParser->result == URLPARSER_VALID_REQUEST) {
		puts("\n\nLOG: Se parsea la url recibida.\n");
			int i, j, k, m;
			for (i = 0; urlParser->url[i] != '/'; i++)
				;
			i++;
			for (j = 0; (urlParser->url[i] != ' ' && urlParser->url[i] != '?'); i++, j++)
				urlParser->path[j] = urlParser->url[i];
			urlParser->path[j] = '\0';
			if (urlParser->url[i] == ' ') {
				urlParser->type[0] = '\0';
				puts("\n\nLOG: Termino de parsear la Url recibidal Url\n");
				return;
			}
			i++;
			for (m = 0; urlParser->url[i] != '='; i++, m++)
				urlParser->type[m] = urlParser->url[i];
			urlParser->type[m] = '\0';
			i++;
			for (k = 0; urlParser->url[i] != ' '; i++, k++)
				if (isHtmlSpace(urlParser->url, &i)) {
					urlParser->queryString[k] = ' ';
				} else {
					urlParser->queryString[k] = urlParser->url[i];
				}
			urlParser->queryString[k] = '\0';
			puts("\n\nLOG: Termino de parsear la Url recibidal Url\n");
	}
}

AbstractUrlParser NewUrlParser(char* url) {
	int maxLen = strlen(url);
	AbstractUrlParser urlParser = (AbstractUrlParser) malloc(
			sizeof(struct ConcreteUrlParser));
	urlParser->url = MemReserveForChar(maxLen);
	urlParser->url = url;
	urlParser->type = MemReserveForChar(maxLen);
	urlParser->path = MemReserveForChar(maxLen);
	urlParser->queryString = MemReserveForChar(maxLen);
	_ParseUrl(urlParser);

	if (urlParser->result == URLPARSER_ERROR_INVALID_REQUEST) {
		return urlParser;
	}

	if (!_HaveValidParam(urlParser)) {
		urlParser->result = URLPARSER_ERROR_INVALID_PARAM;
		return urlParser;
	}
	if (!_HaveValidPath(urlParser)) {
		urlParser->result = URLPARSER_ERROR_INVALID_PATH;
		return urlParser;
	}

	urlParser->result = URLPARSER_SUCCESS;
	return urlParser;
}

char* UrlParserGetPath(AbstractUrlParser urlParser) {
	return urlParser->path;
}

char* UrlParserGetQueryString(AbstractUrlParser urlParser) {
	return urlParser->queryString;
}

char* UrlParserTypeOfParam(AbstractUrlParser urlParser) {
	return urlParser->type;
}

int UrlParserIsGroupPage(AbstractUrlParser urlParser) {
	return !strcmp(GROUP_PAGE1, urlParser->path);
}

int UrlParserIsIndexPage(AbstractUrlParser urlParser) {
	return !strcmp(INDEX_PAGE1, urlParser->path);
}

int UrlParserIsNoticePage(AbstractUrlParser urlParser) {
	return !strcmp(NOTICE_PAGE1, urlParser->path);
}

int UrlParserGetParseResult(AbstractUrlParser urlParser) {
	return urlParser->result;
}

void UrlParserFree(AbstractUrlParser urlParser) {
	free(urlParser->path);
	free(urlParser->queryString);
	free(urlParser->type);
	//NO HACER FREE q la usan afueraaaaaaaaaa
	//free(urlParser->url);
	free(urlParser);
}

