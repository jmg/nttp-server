#include "Logs.h"
#include "Configuration.h"

#define TIMESEPARATOR ":"
#define ENDOFLINE "\n"
#define SPACE " "
#define ENDSESSION "******************************  END SESSION  ******************************"
#define BEGINSESSION "****************************** BEGIN SESSION ******************************"

struct LogFileClass
{
	HANDLE f;
};

LogFile CurrentLogFile()
{
	LogFile log = (LogFile) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(struct LogFileClass));
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
	MemFree(log);
}

void LogFileOpenFile(LogFile log)
{
	log->f = CreateFileA("Logs.txt", FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}

void LogFileCloseFile(LogFile log)
{
	CloseHandle(log->f);
}

void LogWrite(char* text, LogFile log)
{
	DWORD bytes;
	WriteFile(log->f, text, strlen(text), &bytes, NULL);
}

void LogFileWriteSpace(LogFile log)
{
	LogWrite(SPACE, log);
}

void LogFileWriteEndOfline(LogFile log)
{
	LogWrite(ENDOFLINE, log);
}

void LogFileWriteSeparator(LogFile log)
{
	LogWrite(TIMESEPARATOR ,log);
}

void LogFileWriteBeginSession(LogFile log)
{
	LogWrite(BEGINSESSION ,log);
}

void LogFileWriteEndSession(LogFile log)
{
	LogWrite(ENDSESSION ,log);
}

void LogFileLogEntry(char* ProcessName, int ThreadID, const char* LogType, const char* Data, char* Param)
{
	LogFile log;
	char* TimeString = getDate();
	char* Pid = MemReserveForChar(10);
	char* ThreadIDStr = MemReserveForChar(10);
	sprintf(Pid,"%d",0);//getpid());
	sprintf(ThreadIDStr,"%d",ThreadID);

	log = CurrentLogFile();

	LogWrite(TimeString, log);
	LogFileWriteSpace(log);
	LogWrite(ProcessName, log);
	LogFileWriteSpace(log);
	LogWrite(Pid , log);
	if(ThreadID)
	{
		LogFileWriteSpace(log);
		LogWrite(ThreadIDStr , log);
	}
	LogFileWriteSeparator(log);
	LogWrite((char*)LogType, log);
	LogFileWriteSeparator(log);
	LogWrite((char*)Data, log);
	if(Param)
	{
		LogFileWriteSpace(log);
		LogWrite(Param, log);
	}
	LogFileWriteEndOfline(log);

	LogFileCloseFile(log);
	FreeLogFile(log);
	MemFree(Pid);
	MemFree(ThreadIDStr);
	MemFree(TimeString);
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


