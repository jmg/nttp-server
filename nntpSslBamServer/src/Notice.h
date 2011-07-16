#ifndef NOTICE_H
#define NOTICE_H
#include "Utils/Stack.h"
#include <string.h>

#define NONEXTNOTICEID "-10000"

typedef struct ConcreteNotice * AbstractNotice;

AbstractNotice NewNotice(char * id, char * header, char * body,
		char * groupName);

char * getId(AbstractNotice);
char * getHeader(AbstractNotice);
char * getBody(AbstractNotice);
char * getGroupName(AbstractNotice);
void NoticeFree(AbstractNotice notice);
char* NoticeSerialize(AbstractNotice notice);
char* BodySerialize(AbstractNotice notice);
char* HeaderSerialize(AbstractNotice notice);

char* NoticeStackSerialize(AbstractStack stack);


#endif
