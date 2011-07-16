#include "../../nntpServer/News.cpp"
#include "LdapService.cpp"
#include <string>

using namespace std;

class DatabaseService
{
private:
	LdapService ldapService;

public:
	DatabaseService(string host)
	{
		ldapService.CreateInstance(host);
	}

	int uploadNotice(New NewToInsert) 
	{
		if (ldapService.InsertNotice(NewToInsert)!=0)
			return 1;
		return 0;
	}
};


