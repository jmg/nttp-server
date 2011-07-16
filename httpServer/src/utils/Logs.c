#include "Logs.h"

#define TIMESEPARATOR ":"
#define ENDOFLINE "\n"
#define SPACE " "
#define ENDSESSION "******************************  END SESSION  ******************************"
#define BEGINSESSION "****************************** BEGIN SESSION ******************************"

struct LogFileClass
{
	FILE* f;
};

LogFile CurrentLogFile()
{
	LogFile log = (LogFile) malloc(sizeof(struct LogFileClass));
	LogFileOpenFile(log);
	return log;
}

void LogFileLogInitSession()
{
	LogFile log = CurrentLogFile();
	LogFileWriteBeginSession(log);
	LogFileWriteEndOfline(log);
	LogFileWriteEndOfline(log);
	LogFileCloseFile(log);
	FreeLogFile(log);
}

void LogFileLogEndSession()
{
	LogFile log = CurrentLogFile();
	LogFileWriteEndOfline(log);
	LogFileWriteEndSession(log);
	LogFileWriteEndOfline(log);
	LogFileWriteEndOfline(log);
	LogFileCloseFile(log);
	FreeLogFile(log);
}

void FreeLogFile(LogFile log)
{
	free(log);
}

void LogFileOpenFile(LogFile log)
{
	log->f = fopen("Logs.txt","a");
}

void LogFileCloseFile(LogFile log)
{
	fclose(log->f);
}

void LogFileWriteSpace(LogFile log)
{
	fwrite(SPACE ,sizeof(char), strlen(SPACE), log->f);
}

void LogFileWriteEndOfline(LogFile log)
{
	fwrite(ENDOFLINE ,sizeof(char), strlen(ENDOFLINE), log->f);
}

void LogFileWriteSeparator(LogFile log)
{
	fwrite(TIMESEPARATOR ,sizeof(char), strlen(TIMESEPARATOR), log->f);
}

void LogFileWriteBeginSession(LogFile log)
{
	fwrite(BEGINSESSION ,sizeof(char), strlen(BEGINSESSION), log->f);
}

void LogFileWriteEndSession(LogFile log)
{
	fwrite(ENDSESSION ,sizeof(char), strlen(ENDSESSION), log->f);
}

void LogFileLogEntry(char* ProcessName, int ThreadID, const char* LogType, const char* Data, char* Param)
{
	char* TimeString = getDate();
	char* Pid = MemReserveForChar(10);
	char* ThreadIDStr = MemReserveForChar(10);
	sprintf(Pid,"%d",getpid());
	sprintf(ThreadIDStr,"%d",pthread_self());

	LogFile log = CurrentLogFile();

	fwrite(TimeString ,sizeof(char), strlen(TimeString), log->f);
	LogFileWriteSpace(log);
	fwrite(ProcessName ,sizeof(char), strlen(ProcessName), log->f);
	LogFileWriteSpace(log);
	fwrite(Pid ,sizeof(char), strlen(Pid), log->f);
	if(ThreadID)
	{
		LogFileWriteSpace(log);
		fwrite(ThreadIDStr ,sizeof(char), strlen(ThreadIDStr), log->f);
	}
	LogFileWriteSeparator(log);
	fwrite(LogType ,sizeof(char), strlen(LogType), log->f);
	LogFileWriteSeparator(log);
	fwrite(Data ,sizeof(char), strlen(Data), log->f);
	if(Param)
	{
		LogFileWriteSpace(log);
		fwrite(Param ,sizeof(char), strlen(Param), log->f);
	}
	LogFileWriteEndOfline(log);

	LogFileCloseFile(log);
	FreeLogFile(log);
	free(Pid);
	free(ThreadIDStr);
	free(TimeString);
}

char* getDate()
{
	char* timeString = MemReserveForChar(10);
	char* buffer = MemReserveForChar(10);
	time_t secs = time(0);
	struct tm* local = localtime(&secs);
	if(local->tm_hour < 10)
		strcat(timeString, "0");
	sprintf(buffer, "%d", local->tm_hour);
	strcat(timeString, buffer);
	strcat(timeString, ":");
	if(local->tm_min < 10)
		strcat(timeString, "0");
	sprintf(buffer, "%d", local->tm_min);
	strcat(timeString, buffer);
	strcat(timeString, ":");
	if(local->tm_sec < 10)
		strcat(timeString, "0");
	sprintf(buffer, "%d", local->tm_sec);
	strcat(timeString, buffer);
	return timeString;
}
