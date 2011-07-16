#ifndef DATABASESERVICE_H
#define DATABASESERVICE_H
#include "LdapService.h"
#include "../NoticeGroups.h"
#include "../Utils/Stack.h"
#include <stdio.h>

typedef struct ConcreteDatabaseService * AbstractDatabaseService;

AbstractDatabaseService NewDatabaseService(char *, char *);

void uploadNotice(AbstractDatabaseService, AbstractNotice);

AbstractNotice getNoticeById(AbstractDatabaseService, char * id);

AbstractNoticeGroups DatabaseServiceGetNoticesGroups(AbstractDatabaseService);

AbstractStack DatabaseServiceGetNoticesByGroupName(AbstractDatabaseService abstractDatabaseService,
		char * groupName);

AbstractNotice DatabaseServiceGetNoticesById(AbstractDatabaseService abstractDatabaseService,
		char * noticeId);


AbstractNotice DatabaseServiceGetNextNotice(AbstractDatabaseService abstractDatabaseService,
		char * noticeId);

#endif
