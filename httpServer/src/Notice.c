#include "Notice.h"

struct ConcreteNotice {
	char * id;
	char * header;
	char * body;
	char * groupName;
};

AbstractNotice NewNotice(char * id, char * header, char * body,
		char * groupName) {

	AbstractNotice abstractNotice;
	if ((abstractNotice = malloc(sizeof(struct ConcreteNotice)))) {
		abstractNotice->id = id;
		abstractNotice->header = header;
		abstractNotice->body = body;
		abstractNotice->groupName = groupName;
	}
	return abstractNotice;
}

char * getId(AbstractNotice abstractNotice) {
	return abstractNotice->id;
}

char * getHeader(AbstractNotice abstractNotice) {
	return abstractNotice->header;
}

char * getBody(AbstractNotice abstractNotice) {
	return abstractNotice->body;
}

char * getGroupName(AbstractNotice abstractNotice) {
	return abstractNotice->groupName;
}

AbstractNotice convertXmlToNotice(char * noticeXml) {
	char * head = XmlStreamGetParam(noticeXml, "noticeHead");
	char * group = XmlStreamGetParam(noticeXml, "noticeGroup");
	char * body = XmlStreamGetParam(noticeXml, "noticeBody");
	char * id = XmlStreamGetParam(noticeXml, "noticeId");

	return NewNotice(id, head, body, group);
}

char * convertNoticeToXml(AbstractNotice abstractNotice) {
	char * preffix = "<notice>";
	char * suffix = "</notice>";
	char * idPreffix = "<noticeId>";
	char * idSuffix = "</noticeId>";
	char * headPreffix = "<noticeHead>";
	char * headSuffix = "</noticeHead>";
	char * groupPreffix = "<noticeGroup>";
	char * groupSuffix = "</noticeGroup>";
	char * bodyPreffix = "<noticeBody>";
	char * bodySuffix = "</noticeBody>";

	char * xmlNotice = MemReserveForChar(strlen(preffix) + strlen(suffix)
			+ strlen(idPreffix) + strlen(idSuffix) + strlen(headPreffix)
			+ strlen(headSuffix) + strlen(groupPreffix) + strlen(groupSuffix)
			+ strlen(bodyPreffix) + strlen(bodySuffix) + strlen(
			abstractNotice->body) + strlen(abstractNotice->groupName) + strlen(
			abstractNotice->header) + strlen(abstractNotice->id) + 1);

	strcat(xmlNotice, preffix);

	strcat(xmlNotice, idPreffix);
	strcat(xmlNotice, abstractNotice->id);
	strcat(xmlNotice, idSuffix);

	strcat(xmlNotice, headPreffix);
	strcat(xmlNotice, abstractNotice->header);
	strcat(xmlNotice, headSuffix);

	strcat(xmlNotice, groupPreffix);
	strcat(xmlNotice, abstractNotice->groupName);
	strcat(xmlNotice, groupSuffix);

	strcat(xmlNotice, bodyPreffix);
	strcat(xmlNotice, abstractNotice->body);
	strcat(xmlNotice, bodySuffix);

	strcat(xmlNotice, suffix);

	return xmlNotice;
}

void NoticeFree(AbstractNotice notice) {
	free(notice->body);
	free(notice->id);
	free(notice->header);
	free(notice->groupName);
	free(notice);
}
