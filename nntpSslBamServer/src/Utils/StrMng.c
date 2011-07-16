#include "StrMng.h"

void ToUpper(char* s) {
	int i;
	for (i = 0; s[i]; i++) {
		char c = s[i];
		s[i] = toupper(c);
	}
	s[i] = '\0';
}

void concatString(char* str1, char* str2) {
	int i, j;
	for (i = 0; str1[i]; i++)
		i++;
	for (j = 0; str2[j]; j++, i++)
		str1[i] = str2[j];
	str1[i] = '\0';
}
