#ifndef LDAPSERVICE_H
#define LDAPSERVICE_H
#include "wrapper/LdapWrapper.h"
#include "../Utils/Logs.h"
#include "../Notice.h"
#include "../NoticeGroups.h"
#include "../Utils/Stack.h"
#include <stdio.h>

typedef struct ConcreteLdapService * AbstractLdapService;

AbstractLdapService NewLdapService(char *);

AbstractNotice LdapServiceGetNoticeById(AbstractLdapService abstractLdapService, char *);

void LdapServiceInsertNotice(AbstractLdapService abstractLdapService,
		AbstractNotice abstractNotice);

AbstractNoticeGroups LdapServiceGetNoticesGroups(AbstractLdapService abstractLdapService);

AbstractStack LdapServiceGetNoticesByGroupName(AbstractLdapService abstractLdapService,
		char * groupName);

char * LdapServiceGetQuery(char * field, char * value);

AbstractNotice LdapServiceGetNotice(PLDAP_RECORD_OP recordOp,
		PLDAP_RECORD record);

AbstractNotice LdapServiceGetNextNotice(AbstractLdapService abstractLdapService, char* id);


#endif
