#ifndef NNTPCOMMAND_CPP
#define NNTPCOMMAND_CPP
#include <string>
#include "StrMng.cpp"
#include <string.h>

using namespace std;

#define TOOLONGLINE "Line Too Long"
#define ERRORCODE "500"

class NNTPCommand {

public:
	string value;
	static const int MAXLENGTH = 512;

	NNTPCommand() {
	}

	int isExit() {
		return StrMng::ToUpper(value) == "QUIT";
	}

	int isTooLong()
	{
		return value.length() >= MAXLENGTH;
	}

	int isEmpty()
	{
		return value == "";
	}

	string getCommand() {
		return value + "\r\n";
	}
};

#endif
