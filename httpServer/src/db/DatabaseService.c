#include "DatabaseService.h"

struct ConcreteDatabaseService {
	AbstractLdapService abstractLdapService;
	AbstractMemCachedService abstractMemCachedService;
};

AbstractDatabaseService NewDatabaseService(char * dbServerUrl, char * cacheServerUrl, char * chacheServerUrl2, int cachePort1, int cachePort2) {
	AbstractDatabaseService abstractDatabaseService;
	if ((abstractDatabaseService = malloc(
			sizeof(struct ConcreteDatabaseService)))) {
		abstractDatabaseService->abstractMemCachedService
				= NewMemCachedService(cacheServerUrl, cachePort1, chacheServerUrl2, cachePort2);
		abstractDatabaseService->abstractLdapService = NewLdapService(dbServerUrl);
	}
	return abstractDatabaseService;
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

	char * xmlNotice = getCache(abstractDatabaseService->abstractMemCachedService, noticeId);

	AbstractNotice notice;

	if (xmlNotice) {
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se encontro la noticia en memcached y se parsea: ", xmlNotice);
		notice = convertXmlToNotice(xmlNotice);
		free(xmlNotice);
	} else {
		notice =  LdapServiceGetNoticeById(abstractDatabaseService->abstractLdapService, noticeId);
		if (!notice) {
			return NULL;
		}
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se encontro la noticia en OpenDs, la cual va a ser preparada para insertar en memcached con ID: ", noticeId);
		char * xmlNotice = convertNoticeToXml(notice);
		setearCache(abstractDatabaseService->abstractMemCachedService, getId(notice), xmlNotice);
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se inserto la noticia en memcached: ", xmlNotice);
		free(xmlNotice);
	}
	return notice;
}
