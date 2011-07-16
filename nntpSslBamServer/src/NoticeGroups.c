#include "NoticeGroups.h"
#include "Utils/Logs.h"

struct ConcreteNoticeGroups {
	char* groups[1000];
	int groupsCount;
};

AbstractNoticeGroups NewNoticeGroups() {
	AbstractNoticeGroups abstractNoticeGroups;
	if ((abstractNoticeGroups = malloc(sizeof(struct ConcreteNoticeGroups)))) {
		abstractNoticeGroups->groupsCount = 0;
	}
	return abstractNoticeGroups;
}

void NoticeGroupAddNoticeGroup(AbstractNoticeGroups noticeGroups, char * groupName) {
	noticeGroups->groups[noticeGroups->groupsCount] = calloc(strlen(groupName) + 5, sizeof(char));
	strcpy(noticeGroups->groups[noticeGroups->groupsCount], groupName);
	noticeGroups->groupsCount++;
}

char * NoticeGroupGetGroupName(AbstractNoticeGroups noticeGroup, int index) {
	return noticeGroup->groups[index];
}

int NoticeGroupGetGroupsCount(AbstractNoticeGroups noticeGroups) {
	return noticeGroups->groupsCount;
}

void NoticeGroupFree(AbstractNoticeGroups noticeGroups) {
	int i;
	for(i=0;i < noticeGroups->groupsCount;i++)
	{
		if(noticeGroups->groups[i] && strlen(noticeGroups->groups[i]) > 0)
			free(noticeGroups->groups[i]);
	}
	free(noticeGroups);
}

char* NoticeGroupsSerialize(AbstractNoticeGroups noticeGroups)
{
	int i;
	char * serializedGroups = MemReserveForChar(strlen(noticeGroups->groups[0]) + 5);
	strcat(serializedGroups, noticeGroups->groups[0]);
	strcat(serializedGroups, "\n");
	for(i=1; i < noticeGroups->groupsCount ;i++)
	{		
		serializedGroups = MemExtendForChar(serializedGroups, strlen(noticeGroups->groups[i]) + 5);		
		strcat(serializedGroups, noticeGroups->groups[i]);		
		strcat(serializedGroups, "\n");
	}
	NoticeGroupFree(noticeGroups);
	return serializedGroups;
}
