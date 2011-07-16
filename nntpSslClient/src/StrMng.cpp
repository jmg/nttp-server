#ifndef STRMNG_CPP
#define STRMNG_CPP
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace std;

class StrMng {

public:
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
	static string ToLower(string s) {
		char* ToLower = MemReserve(s);
		for (unsigned int i = 0; i < s.length(); i++) {
			ToLower[i] = tolower(s[i]);
		}
		ToLower[s.length()] = '\0';
		return getString(ToLower);
	}
	static string ToUpper(string s) {
		char* ToUpper = MemReserve(s);
		for (unsigned int i = 0; i < s.length(); i++) {
			ToUpper[i] = toupper(s[i]);
		}
		ToUpper[s.length()] = '\0';
		return getString(ToUpper);
	}
	static char* MemReserve(string s) {
		int len = s.length();
		return (char*) calloc(len + 1, sizeof(char));
	}
	static vector<string> Split(string str, string delim) {
		vector<string> elements;
		unsigned int part, i = 0;
		while ((part = str.find_first_of(delim)) != str.npos) {
			if (part > 0) {
				elements.push_back(str.substr(0, part));
				i++;
			}
			str = str.substr(part + 1);
		}
		if (str.length() > 0) {
			elements.push_back(str);
		}
		return elements;

	}
	static string Trim(string str) {
		int firstNonSpace, lastNonSpace;
		firstNonSpace = str.find_first_not_of("[ \t\n]");
		lastNonSpace = str.find_last_not_of("[ \t\n]");
		if (lastNonSpace < 0 || firstNonSpace < 0)
			return str;
		return str.substr(firstNonSpace, lastNonSpace - firstNonSpace + 1);
	}
	static string FindAndDel(string s, string del) {
		unsigned int found = s.find(del);
		if (found == s.npos)
			return s;
		string first = s.substr(0, found);
		string second = s.substr(found + del.length(), s.length() - 1);
		return first + second;
	}

	static void concatString(char* str1, char* str2)
	{
		int i,j;
		for(i=0;str1[i];i++)
			i++;
		for(j=0;str2[j];j++,i++)
			str1[i] = str2[j];
		str1[i] = '\0';
	}

	static string CleanResponse(string str)
	{
		/*string s = FindAndDel(str, "\r\n");
		s = FindAndDel(str, "\n");
		return s;*/
		return str;
	}
};
#endif
