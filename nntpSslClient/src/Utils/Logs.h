#ifndef LOGS_H_
#define LOGS_H_

//Log types
#define LOGTYPEINFO "INFO"
#define LOGTYPEWARN "WARN"
#define LOGTYPEERROR "ERROR"
#define LOGTYPEDEBUG "DEBUG"

#define MAINPROCESSNAME "MAIN"
#define NOTHREADID 0
#define NOPARMS 0

typedef struct LogFileClass *LogFile;


LogFile CurrentLogFile();
void LogFileLogInitSession();
void LogFileLogEndSession();
void FreeLogFile(LogFile log);
void LogFileOpenFile(LogFile log);
void LogFileCloseFile(LogFile log);
void LogFileWriteSpace(LogFile log);
void LogFileWriteEndOfline(LogFile log);
void LogFileWriteSeparator(LogFile log);
void LogFileWriteBeginSession(LogFile log);
void LogFileWriteEndSession(LogFile log);
void LogFileLogEntry(char* ProcessName, int ThreadID, const char* LogType, const char* Data, const char* Param);
char* getDate();

#endif /* LOGS_H_ */
