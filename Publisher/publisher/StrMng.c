#ifndef StrMng_C
#define StrMng_C

#include "StrMng.h"
#include "time.h"
#include "windows.h"
#include "stdio.h"

char* MemReserveForChar(int len)
{
	return (char*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (len+1) * sizeof(char));
}

char* MemExtendForChar(char* mem, int len)
{
	return (char*) HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, mem,sizeof(char) * (len+1));
}

void MemFree(void* mem)
{
    HeapFree(GetProcessHeap(), 0, mem);
}

int GetLine(char** str)
{
	char *temp = MemReserveForChar(1);
	char c;
	int i;
	for(i = 0;((c = getchar()) != '\n');i++)
	{
		 temp[i] = c;
		 temp = MemExtendForChar(temp,i+1);
	}
	temp[i] = '\0';
	*str = MemReserveForChar(i);
	strcpy(*str, temp);
	MemFree(temp);
	return i;
}

void GetLong(long* l)
{	
	scanf("%d", l);
	fflush(stdin);
}

void GetString(char* s)
{	
	strcpy(s," ");
	scanf("%[^\&]", s);
	fflush(stdin);
}

int isEmpty(char* s)
{
	if (strcmp(s," ") == 0) {
		return 1;
	}

	if( strchr(s,'&') != NULL || strchr(s,'<') != NULL || strchr(s,'>') != NULL || strchr(s,'/') != NULL || strchr(s,"'") != NULL  ) {
		return 1;
	}

	return 0;
}

void SubString(char* string, int begin, int end, char** outStr)
{
	int i,j;
	char* temp = MemReserveForChar(end-begin+1);
	for(i = begin,j=0;i<=end;i++,j++)
		temp[j] = string[i];
	*outStr = MemReserveForChar(strlen(temp)+1);
	strcpy(*outStr, temp);
	MemFree(temp);
}

void SubConstString(char* string, int begin, int end, char* outStr)
{
	int i,j;
	for(i = begin,j=0;i<=end;i++,j++)
		outStr[j] = string[i];
}

void StreamConcat(char* s, char* str)
{
	int i,j;
	for(i=0;s[i];i++);
	for(;str[i];i++)
		s[i] = str[i];
}

char* Replace(char* string, char* pattern, char* replace)
{
	int i,j;
	char * temp;
	int len = strlen(string);
	int begin = strstr(string, pattern) - string;
	int end = begin + strlen(pattern);	
	int replaceLen = strlen(replace);			
	SubString(string, end, len, &temp);
	for(i = begin,j = 0;i <= begin+replaceLen;i++,j++)
		string[i] = replace[j];	
	string[i] = '\0';
	strcat(string,temp);
	MemFree(temp);
	return string;
}

char* LongToString(long l)
{
	char* buffer = MemReserveForChar(10);
	sprintf(buffer, "%d", l);
	return buffer;
}

long Random()
{	
	long uniqueId;
	/*srand((unsigned) time(0));	
	return rand();*/
	char time[10];
	SYSTEMTIME SystemTime;
	GetSystemTime(&SystemTime); 
	sprintf(time , "%02d%02d%03d", SystemTime.wMinute, SystemTime.wSecond, SystemTime.wMilliseconds);
	uniqueId = atoi(time);
	return uniqueId;
}

HRESULT __fastcall AnsiToUnicode(LPCSTR pszA, LPOLESTR* ppszW)
{
    ULONG cCharacters;
    DWORD dwError;

    if (NULL == pszA)
    {
        *ppszW = NULL;
        return NOERROR;
    }
    cCharacters =  strlen(pszA)+1;

    *ppszW = (LPOLESTR) CoTaskMemAlloc(cCharacters*2);
    if (NULL == *ppszW)
        return E_OUTOFMEMORY;

    if (0 == MultiByteToWideChar(CP_ACP, 0, pszA, cCharacters,
                  *ppszW, cCharacters))
    {
        dwError = GetLastError();
        CoTaskMemFree(*ppszW);
        *ppszW = NULL;
        return HRESULT_FROM_WIN32(dwError);
    }
    return NOERROR;
}

#endif