#ifndef LDAPSERVICE_H
#define LDAPSERVICE_H
#include "LdapWrapper/LdapWrapper.h"
#include "../Notice.h"
#include "../NoticeGroups.h"
#include "../utils/Stack.h"
#include <stdio.h>
#include "../utils/Logs.h"

typedef struct ConcreteLdapService * AbstractLdapService;

AbstractLdapService NewLdapService(char *);

AbstractNotice LdapServiceGetQueryGetNoticeById(AbstractLdapService abstractLdapService, char *);

void LdapServiceInsertNotice(AbstractLdapService abstractLdapService,
		AbstractNotice abstractNotice);

AbstractNoticeGroups LdapServiceGetNoticesGroups(AbstractLdapService abstractLdapService);

AbstractStack LdapServiceGetNoticesByGroupName(AbstractLdapService abstractLdapService,
		char * groupName);

AbstractNotice LdapServiceGetNoticeById(
		AbstractLdapService abstractLdapService, char * id);

#endif
