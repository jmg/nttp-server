#include "DatabaseService.h"

struct ConcreteDatabaseService {
	AbstractLdapService abstractLdapService;
	AbstractMemCachedService abstractMemCachedService;
};

AbstractDatabaseService NewDatabaseService(char * dbServerUrl, char * cacheServerUrl) {
	AbstractDatabaseService abstractDatabaseService;
	if ((abstractDatabaseService = malloc(
			sizeof(struct ConcreteDatabaseService)))) {
		/*abstractDatabaseService->abstractMemCachedService
				= NewMemCachedService(host);*/
		abstractDatabaseService->abstractLdapService = NewLdapService(dbServerUrl);
	}
	return abstractDatabaseService;
}

AbstractNotice getNoticeById(AbstractDatabaseService abstractDatabaseService, char * id) {
	return LdapServiceGetNotice(abstractDatabaseService->abstractLdapService, id);
}

void uploadNotice(AbstractDatabaseService abstractDatabaseService, AbstractNotice notice) {
	LdapServiceInsertNotice(abstractDatabaseService->abstractLdapService, notice);
}

AbstractNoticeGroups DatabaseServiceGetNoticesGroups(AbstractDatabaseService abstactDatabaseService) {
	return LdapServiceGetNoticesGroups(
			abstactDatabaseService->abstractLdapService);
}

AbstractStack DatabaseServiceGetNoticesByGroupName(AbstractDatabaseService abstractDatabaseService,
		char * groupName) {
	return LdapServiceGetNoticesByGroupName(abstractDatabaseService->abstractLdapService, groupName);
}

AbstractNotice DatabaseServiceGetNoticesById(AbstractDatabaseService abstractDatabaseService,
		char * noticeId) {
	return LdapServiceGetNoticeById(abstractDatabaseService->abstractLdapService, noticeId);
}
