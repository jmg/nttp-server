#ifndef NOTICEGROUPS_H
#define NOTICEGROUPS_H
#include <stdio.h>

typedef struct ConcreteNoticeGroups * AbstractNoticeGroups;

void NoticeGroupAddNoticeGroup(AbstractNoticeGroups noticeGroups, char * groupName);

char * NoticeGroupGetGroupName(AbstractNoticeGroups noticeGroup, int index);

int NoticeGroupGetGroupsCount(AbstractNoticeGroups noticeGroups);

void NoticeGroupFree(AbstractNoticeGroups noticeGroups);

#endif
