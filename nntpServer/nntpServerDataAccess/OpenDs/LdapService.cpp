#include "../../nntpServer/News.cpp"
#include "LdapWrapper/LdapWrapper.h"
#include "../../nntpServer/Utils/Logs.h"
#include <string>
#include <sstream>

using namespace std;

class LdapService
{
public:
	string serverUrl;
	PLDAP_CONTEXT context;
	PLDAP_CONTEXT_OP ctxOp;
	PLDAP_SESSION_OP sessionOp;
	PLDAP_ENTRY_OP entryOp;
	PLDAP_ATTRIBUTE_OP attribOp;

	LdapService()
	{
	}

	void CreateInstance(string Host)
	{
		serverUrl = Host;
	}
	
	void initializeContext() 
	{
		context = newLDAPContext();
		ctxOp = newLDAPContextOperations();
		sessionOp = newLDAPSessionOperations();
		entryOp = newLDAPEntryOperations();
		attribOp = newLDAPAttributeOperations();		
		ctxOp->initialize(context, (PCHAR)serverUrl.c_str());
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se inizializo el contexto de ldap", NOPARMS);
	}
	
	void closeContext() 
	{
		freeLDAPContext(context);
		freeLDAPContextOperations(ctxOp);
		freeLDAPSessionOperations(sessionOp);
	}

	PLDAP_SESSION getSession() 
	{
		PLDAP_SESSION session = ctxOp->newSession(context, "cn=Directory Manager", "password");
		sessionOp->startSession(session);
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se inizializo session de ldap", NOPARMS);

		return session;
	}

	void closeSession(PLDAP_SESSION session) 
	{
		sessionOp->endSession(session);
		freeLDAPSession(session);
	}

	int InsertNotice(New abstractNotice) 
	{
		stringstream strstream;
		PLDAP_SESSION session;
		PLDAP_ENTRY entry;
		string suffix = ",ou=so,dn=utn,dn=edu";
		string prefix = "utnArticleID=";
		string articleId;
		string id;

		initializeContext();
		
		session = getSession();
		entry = entryOp->createEntry();

		articleId.append(prefix);						

		strstream << abstractNotice.IdNew;
		strstream >> id;

		articleId.append(id);
		articleId.append(suffix);

		entry->dn = (PCHAR)articleId.c_str();

		entryOp->addAttribute(entry, attribOp->createAttribute("objectclass", 2, "top", "utnArticle"));
		entryOp->addAttribute(entry, attribOp->createAttribute("utnArticleID", 1, id));
		entryOp->addAttribute(entry, attribOp->createAttribute("utnArticleHead", 1, abstractNotice.Head));
		entryOp->addAttribute(entry, attribOp->createAttribute("utnArticleBody", 1, abstractNotice.Body));
		entryOp->addAttribute(entry, attribOp->createAttribute("utnArticleGroupName", 1, abstractNotice.NewsGroup));

		if (sessionOp->addEntry(session, entry)!=0)
			return 1;

		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se guardo una noticia en ldap", NOPARMS);

		closeSession(session);
		closeContext();
		return 0;
	}
};
