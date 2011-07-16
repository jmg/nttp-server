#ifndef NOTICEGROUPS_H
#define NOTICEGROUPS_H
#include <stdio.h>
#include "string.h"

typedef struct ConcreteNoticeGroups * AbstractNoticeGroups;

AbstractNoticeGroups NewNoticeGroups();

void NoticeGroupAddNoticeGroup(AbstractNoticeGroups noticeGroups, char * groupName);

char * NoticeGroupGetGroupName(AbstractNoticeGroups noticeGroup, int index);

int NoticeGroupGetGroupsCount(AbstractNoticeGroups noticeGroups);

void NoticeGroupFree(AbstractNoticeGroups noticeGroups);

char* NoticeGroupsSerialize(AbstractNoticeGroups noticeGroups);

#endif
