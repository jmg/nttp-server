#include <windows.h>
#include <strsafe.h>

char* MemReserveForChar(int len);
char* MemExtendForChar(char* mem, int len);
void MemFree(void* mem);
int GetLine(char** str);
void GetLong(long* l);
void GetString(char* s);
void SubString(char* string, int begin, int end, char** outStr);
void SubConstString(char* string, int begin, int end, char* outStr);
void StreamConcat(char* s, char* str);
char* Replace(char* string, char* pattern, char* replace);
HRESULT __fastcall AnsiToUnicode(LPCSTR pszA, LPOLESTR* ppszW);
