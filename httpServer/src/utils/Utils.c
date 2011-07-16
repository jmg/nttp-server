#include "Utils.h"

char* MemReserveForChar(int len) {
	return (char*) calloc(len + 1, sizeof(char));
}


char* MemExtendForChar(char* mem, int len)
{
	return (char*) realloc(mem, (strlen(mem) + len + 1) * sizeof(char));
}

char* SubString(char* string, int begin, int end) {
	int i, j;
	char* SubStr = MemReserveForChar(end - begin + 1);
	for (i = begin, j = 0; i <= end; i++, j++) {
		SubStr[j] = string[i];
	}
	SubStr[j] = '\0';
	return SubStr;
}

void concatString(char* str1, char* str2)
{
	int i,j;
	for(i=0;str1[i];i++)
		i++;
	for(j=0;str2[j];j++,i++)
		str1[i] = str2[j];
	str1[i] = '\0';
}
