#ifndef NNTPPARSER_H_
#define NNTPPARSER_H_
#include "string.h"
#include "ctype.h"
#include "Notice.h"
#include "NoticeGroups.h"
#include "OpenDs/DatabaseService.h"
#include "stdlib.h"
#include "Utils/Config.h"
#include "Utils/Logs.h"
#include "Utils/StrMng.h"


typedef struct NNTPParserClass * NNTPParser;
typedef struct NNTPCommmandClass * NNTPCommand;

NNTPParser NewNNTPParser(void);
NNTPCommand NNTPParserParseCommand(NNTPParser nntpParser);
int NNTPParserErrors(NNTPParser nntpParser);
char* NNTPCommandSetResponse(NNTPCommand nntpCommand, AbstractDatabaseService databaseService);
NNTPCommand NewNNTPCommand(void);
NNTPParser NNTPParserLoadRawCommand(NNTPParser nntpParser, char* command);
int MatchCommand(NNTPCommand nntpCommand, char* command);
char* NNTPCommandResponse(NNTPCommand nntpCommand, char* Content, const char* Code);
char* NNTPCommandDefinedResponse(NNTPCommand nntpCommand, const char* Content, const char* Code);
int NNTPCommandIsQuitCommand(NNTPCommand nntpCommand);
void FreeNNTPParser(NNTPParser nntpParser);
NNTPCommand NewNNTPCommand(void);
void FreeNNTPCommand(NNTPCommand nntpCommand);

/****************************** NNTP Commands **********************************/

#define BODY "BODY"
#define GROUP "GROUP"
#define HEAD "HEAD"
#define NEXT "NEXT"
#define ARTICLE "ARTICLE"
#define QUIT "QUIT"
#define LISTGROUP "LISTGROUP"
#define LISTNEWSGROUPS "LIST"
#define POST "POST"

/****************************** NNTP Params **********************************/

#define NEWSGROUPS "NEWSGROUPS"

/****************************** Defined Responses **********************************/

#define WHAT "What?"
#define BYE "Bye!"
#define NOTDONE "Not Found"
#define SYNTAXERROR "Syntax Error"
#define NOTINANEWSGROUP "Not in a Newsgroup"
#define END "."
#define TOOLONGLINE "Line Too Long"
#define POSTMESSAGE "Not Possible"
#define UNKOWNLISCOMMAND "Unknow List Command"
#define NOGROUPSPECIFIED "No Group Specified"
#define NOSUCHGROUP "No Such Group"
#define NONEXT "No next article to retrieve"

/****************************** Codes **********************************/

#define BYECODE "205"
#define ERRORCODE "500"
#define NOTDONECODE "400"
#define OKCODE "200"
#define POSTCODE "440"
#define LISTGROUPOKCODE "211"
#define UNKOWNLISCOMMANDCODE "501"
#define NOGROUPSPECIFIEDCODE "412"
#define NONEXTCODE "412"
#define NOSUCHGROUPCODE "411"
#define SINTAXERRORCODE "501"

/****************************** Sintax Errors **********************************/

#define SINTAXOFHEAD "Syntax is:  HEAD [message-ID|number]"
#define SINTAXOFBODY "Syntax is:  BODY [message-ID|number]"
#define SINTAXOFARTICLE "Syntax is:  ARTICLE [message-ID|number]"
#define SINTAXOFNEXT "Syntax is:  NEXT (no argument allowed)"
#define SINTAXOFQUIT "Syntax is:  QUIT (no argument allowed)"
#define SINTAXOFPOST "Syntax is:  POST (no argument allowed)"
#define SINTAXERORR "Syntax error"

#endif /* NNTPPARSER_H_ */
