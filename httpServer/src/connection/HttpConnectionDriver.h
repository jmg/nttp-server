#ifndef HTTPCONNECTIONDRIVER_H
#define HTTPCONNECTIONDRIVER_H
#include "ClientConnection.h"
#include "../db/DatabaseService.h"
#include "../Notice.h"
#include "../utils/UrlParser.h"
#include "../utils/Utils.h"
#include "../utils/Logs.h"
#include <stdio.h>

typedef struct ConcreteHttpConnectionDriver * AbstractHttpConnectionDriver;

AbstractHttpConnectionDriver NewHttpConnectionDriver(char* baseUrl,
		AbstractDatabaseService abstractDatabaseService,
		AbstractClientConnection clientConnection);

void start(AbstractHttpConnectionDriver httpDriver);

void HttpConnectionDriverFree(AbstractHttpConnectionDriver httpDriver);

#endif
