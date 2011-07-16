#include "NoticeGroups.h"

struct ConcreteNoticeGroups {
	char * groups[1000];
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
	noticeGroups->groups[noticeGroups->groupsCount] = calloc (strlen(groupName) + 2, sizeof(char));
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
	int var;
	for (var = 0; var < noticeGroups->groupsCount; ++var) {
		if (noticeGroups->groups[var] && strlen(noticeGroups->groups[var]) > 0) {
			free(noticeGroups->groups[var]);
		}
	}
	free(noticeGroups);
}
