#ifndef NOTICE_H
#define NOTICE_H
#include <string.h>

typedef struct ConcreteNotice * AbstractNotice;

AbstractNotice NewNotice(char * id, char * header, char * body,
		char * groupName);

char * getId(AbstractNotice);

char * getHeader(AbstractNotice);

char * getBody(AbstractNotice);

char * getGroupName(AbstractNotice);

void NoticeFree(AbstractNotice notice);

AbstractNotice convertXmlToNotice(char * noticeXml);

char * convertNoticeToXml(AbstractNotice abstractNotice);

#endif
