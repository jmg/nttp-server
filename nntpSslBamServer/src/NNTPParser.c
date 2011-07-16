#include "NNTPParser.h"

char* lastNoticeId = NULL;
char* currentGroupName = NULL;

struct NNTPParserClass
{
	char* rawCommand;
	char* ErrorMessage;
	int Errors;
	int MAXLENGTH;
};

struct NNTPCommmandClass
{
	char* Command;
	char* Params;
	char* Response;
};

NNTPParser NewNNTPParser(void)
{
	NNTPParser nntpParser = (NNTPParser) malloc (sizeof(struct NNTPParserClass));
	nntpParser->MAXLENGTH = 512;
	nntpParser->Errors = 0;
	nntpParser->rawCommand = MemReserveForChar(nntpParser->MAXLENGTH);
	nntpParser->ErrorMessage = NULL;
	return nntpParser;
}

NNTPParser NNTPParserLoadRawCommand(NNTPParser nntpParser, char* command)
{
	if(strlen(command) > nntpParser->MAXLENGTH)
	{
		nntpParser->Errors = 1;
		nntpParser->rawCommand = NULL;
		return nntpParser;
	}
	strcpy(nntpParser->rawCommand, command);
	free(command);
	return nntpParser;
}

char* NNTPParserTooLongLineMessage(NNTPParser nntpParser)
{
	nntpParser->ErrorMessage = MemReserveForChar(strlen(TOOLONGLINE) + strlen (ERRORCODE));
	strcat(nntpParser->ErrorMessage, ERRORCODE);
	strcat(nntpParser->ErrorMessage, " ");
	strcat(nntpParser->ErrorMessage, TOOLONGLINE);
	return nntpParser->ErrorMessage;
}

int NNTPParserErrors(NNTPParser nntpParser)
{
	return nntpParser->Errors;
}

void FreeNNTPParser(NNTPParser nntpParser)
{
	if(nntpParser->ErrorMessage)
		free(nntpParser->ErrorMessage);
	if(nntpParser->rawCommand)
		free(nntpParser->rawCommand);
	free(nntpParser);

}
NNTPCommand NewNNTPCommand(void)
{
	NNTPCommand nntpCommand = (NNTPCommand) malloc (sizeof(struct NNTPCommmandClass));
	return nntpCommand;
}

void FreeNNTPCommand(NNTPCommand nntpCommand)
{
	if (nntpCommand->Params && strlen(nntpCommand->Params) > 0)
		free(nntpCommand->Command);
	if (nntpCommand->Params && strlen(nntpCommand->Params) > 0)
		free(nntpCommand->Params);
	free(nntpCommand->Response);
	free(nntpCommand);
}

int isFinishOfCommand(char* RawCommand, int i)
{
	if (!RawCommand[i] || RawCommand[i] == '\r' && RawCommand[i+1] == '\n' && !RawCommand[i+2])
		return 1;
	return 0;
}

NNTPCommand NNTPParserParseCommand(NNTPParser nntpParser)
{
	int i, k;

	NNTPCommand nntpCommand = NewNNTPCommand();
	for(i=0; !isspace(nntpParser->rawCommand[i]) && nntpParser->rawCommand[i] && i < nntpParser->MAXLENGTH; i++) ;
	if(i > nntpParser->MAXLENGTH)
		return NULL;
	else if (!nntpParser->rawCommand[i])
		return nntpCommand;
	nntpCommand->Command = MemReserveForChar(i + 5);
	strncpy(nntpCommand->Command, nntpParser->rawCommand, i);
	nntpCommand->Command[i] = '\0';
	i++;
	k = i;

	for(; !isFinishOfCommand(nntpParser->rawCommand, i) && i < nntpParser->MAXLENGTH; i++) ;
	if(i > nntpParser->MAXLENGTH)
		return NULL;
	nntpCommand->Params = MemReserveForChar(i - k + 5);
	nntpCommand->Params = SubString(nntpParser->rawCommand,k,i-1);
	
	return nntpCommand;
}

int MatchCommand(NNTPCommand nntpCommand, char* command)
{
	ToUpper(nntpCommand->Command);
	if (!strcmp(nntpCommand->Command, command))
		return 1;
	return 0;
}

int MatchParam(NNTPCommand nntpCommand, char* param)
{
	ToUpper(nntpCommand->Params);
	if (!strcmp(nntpCommand->Params, param))
		return 1;
	return 0;
}

char* NNTPCommandResponse(NNTPCommand nntpCommand, char* Content, const char* Code)
{
	nntpCommand->Response = MemReserveForChar(strlen(Content) + strlen(END) + strlen(Code) + 5);
	strcat(nntpCommand->Response, Code);
	strcat(nntpCommand->Response, " ");
	strcat(nntpCommand->Response, Content);
	strcat(nntpCommand->Response, END);
	strcat(nntpCommand->Response, "\n");
	free(Content);
	return nntpCommand->Response;
}

char* NNTPCommandDefinedResponse(NNTPCommand nntpCommand, const char* Content, const char* Code)
{
	nntpCommand->Response = MemReserveForChar(strlen(Content) + strlen(Code) +5);
	strcat(nntpCommand->Response, Code);
	strcat(nntpCommand->Response, " ");
	strcat(nntpCommand->Response, Content);
	strcat(nntpCommand->Response, "\r");
	strcat(nntpCommand->Response, "\n");
	return nntpCommand->Response;
}

int NNTPCommandIsQuitCommand(NNTPCommand nntpCommand)
{
	return MatchCommand(nntpCommand, QUIT);
}

void NNTPCommandSaveCurrentNoticeId(AbstractNotice notice)
{
	if(!notice)
	{
		if(lastNoticeId)
		{
			free(lastNoticeId);
			lastNoticeId = NULL;
			return;
		}
		return;
	}
	lastNoticeId = MemReserveForChar(strlen(getId(notice)));
	strcpy(lastNoticeId, getId(notice));
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "El Current Notice Id es: ", lastNoticeId);
}

void NNTPCommandSaveCurrentGroupName(NNTPCommand nntpCommand)
{
	currentGroupName = MemReserveForChar(strlen(nntpCommand->Params));
	strcpy(currentGroupName, nntpCommand->Params);
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "El Current Group es: ", currentGroupName);
}

char* NNTPCommandSetResponse(NNTPCommand nntpCommand, AbstractDatabaseService databaseService)
{
	AbstractNotice abstractNotice;
	AbstractNoticeGroups abstractNoticeGroups;
	AbstractStack abstractStack;

	if(MatchCommand(nntpCommand, BODY))
	{
		if (nntpCommand->Params)
		{
			if(currentGroupName)
			{
				abstractNotice = DatabaseServiceGetNoticesById(databaseService, nntpCommand->Params);
				if (!abstractNotice)
				{
					return NNTPCommandDefinedResponse(nntpCommand, NOTDONE, NOTDONECODE);
				}
				NNTPCommandSaveCurrentNoticeId(abstractNotice);
				return NNTPCommandResponse(nntpCommand, BodySerialize(abstractNotice), OKCODE);
			}
			return NNTPCommandDefinedResponse(nntpCommand, NOTINANEWSGROUP, NOGROUPSPECIFIEDCODE);
		}
		return NNTPCommandDefinedResponse(nntpCommand, SINTAXOFBODY, SINTAXERRORCODE);
	}
	else if(MatchCommand(nntpCommand, HEAD))
	{
		if (nntpCommand->Params)
		{
			if(currentGroupName)
			{
				AbstractNotice abstractNotice = DatabaseServiceGetNoticesById(databaseService, nntpCommand->Params);
				if (!abstractNotice)
				{
					return NNTPCommandDefinedResponse(nntpCommand, NOTDONE, NOTDONECODE);
				}
				NNTPCommandSaveCurrentNoticeId(abstractNotice);
				return NNTPCommandResponse(nntpCommand, HeaderSerialize(abstractNotice), OKCODE);
			}
			return NNTPCommandDefinedResponse(nntpCommand, NOTINANEWSGROUP, NOGROUPSPECIFIEDCODE);
		}
		return NNTPCommandDefinedResponse(nntpCommand, SINTAXOFHEAD, SINTAXERRORCODE);
	}
	else if(MatchCommand(nntpCommand, ARTICLE))
	{
		if (nntpCommand->Params)
		{
			if(currentGroupName)
			{
				AbstractNotice abstractNotice = DatabaseServiceGetNoticesById(databaseService, nntpCommand->Params);
				if (!abstractNotice)
				{
					return NNTPCommandDefinedResponse(nntpCommand, NOTDONE, NOTDONECODE);
				}
				NNTPCommandSaveCurrentNoticeId(abstractNotice);
				return NNTPCommandResponse(nntpCommand, NoticeSerialize(abstractNotice), OKCODE);
			}
			return NNTPCommandDefinedResponse(nntpCommand, NOTINANEWSGROUP, NOGROUPSPECIFIEDCODE);
		}
		return NNTPCommandDefinedResponse(nntpCommand, SINTAXOFARTICLE, SINTAXERRORCODE);
	}
	else if(MatchCommand(nntpCommand, NEXT))
	{
		if(nntpCommand->Params)
		{
			if (lastNoticeId)
			{
				AbstractNotice abstractNotice = DatabaseServiceGetNextNotice(databaseService, lastNoticeId);
				if (!abstractNotice)
				{
					return NNTPCommandDefinedResponse(nntpCommand, NOTDONE, NOTDONECODE);
				}
				if(!strcmp(getId(abstractNotice), NONEXTNOTICEID))
				{
					return NNTPCommandDefinedResponse(nntpCommand, NONEXT, NONEXTCODE);
				}
				NNTPCommandSaveCurrentNoticeId(abstractNotice);
				return NNTPCommandResponse(nntpCommand, NoticeSerialize(abstractNotice), OKCODE);
			}
			return NNTPCommandDefinedResponse(nntpCommand, NOTINANEWSGROUP, NOGROUPSPECIFIEDCODE);
		}
		return NNTPCommandDefinedResponse(nntpCommand, SINTAXOFNEXT, SINTAXERRORCODE);
	}
	else if(MatchCommand(nntpCommand, QUIT))
	{
		if(MatchParam(nntpCommand, "\n"))
		{
			return NNTPCommandDefinedResponse(nntpCommand, BYE, BYECODE);
		}
		return NNTPCommandDefinedResponse(nntpCommand, SINTAXOFQUIT, SINTAXERRORCODE);
	}
	else if(MatchCommand(nntpCommand, LISTGROUP))
	{
		if (nntpCommand->Params)
		{
			abstractStack = DatabaseServiceGetNoticesByGroupName(databaseService, nntpCommand->Params);
			if(!abstractStack)
			{
				return NNTPCommandDefinedResponse(nntpCommand, NOSUCHGROUP, NOSUCHGROUPCODE);
			}
			NNTPCommandSaveCurrentGroupName(nntpCommand);
			NNTPCommandSaveCurrentNoticeId((AbstractNotice) StackTope(abstractStack));
			return NNTPCommandResponse(nntpCommand, NoticeStackSerialize(abstractStack) , LISTGROUPOKCODE);
		}
		return NNTPCommandDefinedResponse(nntpCommand, NOGROUPSPECIFIED, NOGROUPSPECIFIEDCODE);
	}
	else if(MatchCommand(nntpCommand, LISTNEWSGROUPS))
	{
		if (MatchParam(nntpCommand, NEWSGROUPS))
		{
			abstractNoticeGroups = DatabaseServiceGetNoticesGroups(databaseService);
			if ((void *)abstractNoticeGroups == -1)
			{
				return NULL;
			}
			if(!abstractNoticeGroups)
			{
				return NNTPCommandDefinedResponse(nntpCommand, NOSUCHGROUP, NOSUCHGROUPCODE);
			}
			return NNTPCommandResponse(nntpCommand, NoticeGroupsSerialize(abstractNoticeGroups), OKCODE);
		}
		return NNTPCommandDefinedResponse(nntpCommand, UNKOWNLISCOMMAND, UNKOWNLISCOMMANDCODE);
	}
	else if(MatchCommand(nntpCommand, POST))
	{
		if(nntpCommand->Params)
		{
			return NNTPCommandDefinedResponse(nntpCommand, POSTMESSAGE, POSTCODE);
		}
		return NNTPCommandDefinedResponse(nntpCommand, SINTAXOFPOST, SINTAXERRORCODE);
	}

	return NNTPCommandDefinedResponse(nntpCommand, WHAT, ERRORCODE);
}
