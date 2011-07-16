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
	if (abstractNotice = malloc(sizeof(struct ConcreteNotice))) {
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

void NoticeFree(AbstractNotice notice) {
	free(notice->body);
	free(notice->id);
	free(notice->header);
	free(notice->groupName);
	free(notice);
}

char* BodySerialize(AbstractNotice notice)
{
	char* SerializedBody = MemReserveForChar(strlen(notice->body) + strlen("body") + strlen(notice->id) + 4);
	strcat(SerializedBody, notice->id);
	strcat(SerializedBody, " ");
	strcat(SerializedBody, "body");
	strcat(SerializedBody, "\n");
	strcat(SerializedBody, notice->body);
	NoticeFree(notice);
	return SerializedBody;
}

char* HeaderSerialize(AbstractNotice notice)
{
	char* SerializedHeader = MemReserveForChar(strlen(notice->header) + strlen("head") + strlen(notice->id) + 4);
	strcat(SerializedHeader, notice->id);
	strcat(SerializedHeader, " ");
	strcat(SerializedHeader, "head");
	strcat(SerializedHeader, "\n");
	strcat(SerializedHeader, notice->header);
	NoticeFree(notice);
	return SerializedHeader;
}

char* NoticeSerialize(AbstractNotice notice)
{
	char* SerializedNotice = MemReserveForChar(strlen(notice->id) + strlen(notice->header) + strlen(notice->body) + strlen("article") + 9);
	strcat(SerializedNotice, notice->id);
	strcat(SerializedNotice, " ");
	strcat(SerializedNotice, "article");
	strcat(SerializedNotice, "\n");
	strcat(SerializedNotice, notice->header);
	strcat(SerializedNotice, "\n");
	strcat(SerializedNotice, "\n");
	strcat(SerializedNotice, notice->body);
	strcat(SerializedNotice, "\n");
	NoticeFree(notice);
	return SerializedNotice;
}

char* NoticeStackSerialize(AbstractStack stack)
{
	int i;
	int cant = StackGetSize(stack);
	char* cantStr = MemReserveForChar(10);
	sprintf(cantStr, "%d", cant);
	char* SerializedStack = MemReserveForChar(strlen(cantStr) +  2);

	strcat(SerializedStack, cantStr);
	strcat(SerializedStack, " ");
	free(cantStr);

	AbstractNotice LastNotice = (AbstractNotice) StackTope(stack);
	SerializedStack = MemExtendForChar(SerializedStack, strlen(LastNotice->id)+2);
	strcat(SerializedStack, LastNotice->id);
	strcat(SerializedStack, " ");

	AbstractNotice FirstNotice = (AbstractNotice) StackGetFirst(stack);
	SerializedStack = MemExtendForChar(SerializedStack, strlen(FirstNotice->id)+2);
	strcat(SerializedStack, FirstNotice->id);
	strcat(SerializedStack, " ");

	SerializedStack = MemExtendForChar(SerializedStack, strlen(FirstNotice->groupName)+2);
	strcat(SerializedStack, FirstNotice->groupName);
	strcat(SerializedStack, "\n");

	while(!StackIsEmpty(stack))
	{
		AbstractNotice notice = (AbstractNotice) StackPop(stack);
		SerializedStack = MemExtendForChar(SerializedStack, strlen(notice->id) + 3);
		strcat(SerializedStack, notice->id);
		strcat(SerializedStack, "\n");
		NoticeFree(notice);
	}
	StackFree(stack);
	return SerializedStack;
}
