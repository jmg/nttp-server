#ifndef URLPARSER_H
#define URLPARSER_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define URLPARSER_ERROR_INVALID_PARAM  2
#define URLPARSER_ERROR_INVALID_PATH  3
#define URLPARSER_ERROR_INVALID_REQUEST  4
#define URLPARSER_SUCCESS  5

typedef struct  ConcreteUrlParser * AbstractUrlParser;

AbstractUrlParser NewUrlParser(char* url);

char* UrlParserGetPath(AbstractUrlParser urlParser);

char* UrlParserGetQueryString(AbstractUrlParser urlParser);

char* UrlParserTypeOfParam(AbstractUrlParser urlParser);

int UrlParserIsGroupPage(AbstractUrlParser urlParser);

int UrlParserIsIndexPage(AbstractUrlParser urlParser);

int UrlParserIsNoticePage(AbstractUrlParser urlParser);

void UrlParserFree(AbstractUrlParser urlParser);

int UrlParserGetParseResult(AbstractUrlParser urlParser);

#endif
