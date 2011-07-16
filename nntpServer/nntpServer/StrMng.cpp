#ifndef StrMng_CPP
#define StrMng_CPP

#include <windows.h>
#include <string>
#include <stdio.h>

using namespace std;

class StrMng
{
	public:
		StrMng()
		{
		}
		static string SubString(string str, int begin, int end)
		{
			return str.substr(begin, end - begin);
		}
		static int SubStringToInt(string str, int begin, int end)
		{
			string s = str.substr(begin, end - begin);
			return atoi(s.c_str());
		}
		static long SubStringToLong(string str, int begin, int end)
		{
			string s = str.substr(begin, end - begin);
			return atol(s.c_str());
		}
		static void SubStringToCharArray(string str, int begin, int end, char arrayofchar[])
		{
			int i;
			string s = str.substr(begin, end - begin);
			for(i = 0;s[i];i++)
				arrayofchar[i] = s[i];
		}

			static string getString(char* s, bool freeMem) {
		string cadena;
		for (unsigned int i = 0; i < strlen(s); i++) {
			cadena.append(sizeof(char), s[i]);
		}
		if (freeMem) {
			free(s);
		}
		return cadena;
		}

		static string getString(char* s) {
			return getString(s, true);
		}

		static char* ToCharArray(string s) {
			char* chars = MemReserve(s);
			for (unsigned int i = 0; i < s.length(); i++) {
				chars[i] = s[i];
			}
			chars[s.length()] = '\0';
			return chars;
		}

		static char* MemReserve(string s) {
			int len = s.length();
			return (char*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, len + 1);
		}
};

#endif