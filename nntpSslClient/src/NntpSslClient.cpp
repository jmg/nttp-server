#include "Console.cpp"
#include "NNTPCommand.cpp"
#include "Semaphore.cpp"
#include "SharedMem.cpp"
#include "Thread.cpp"
#include "Socket.cpp"
#include "SSLSocket.cpp"
#include "Utils/Config.h"
#include "Utils/Logs.h"
#include <signal.h>

#define SOCKETERROR -1

using namespace std;

void* connector(void *);
void* userInterface(void *);

char* PathToConfig;
int Finish = 0;

Semaphore semUserInterface(0);
Semaphore semConnector(0);
Thread thGui;
Thread thUi;
SharedMem shared;
XmlFile xmlFile;

void Terminate(int s)
{
	Console::WriteLine("El servidor esta fuera de servicio. Bye!");
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "El servidor esta fuera de servicio. Bye!", NOPARMS);
	exit(1);
}

int main(int argc, char* argv[]) {
	LogFileLogInitSession();
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se Inicio el programa", NOPARMS);
	if (argc != 2)
	{
		puts("No se pasaron los argumentos adecuados");
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "No se pasaron los argumentos adecuados", NOPARMS);
		exit(1);
	}
	signal(SIGPIPE, Terminate);
	PathToConfig = argv[1];

	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se Cargo el Siguiente Archivo de Config: ", argv[1]);

	thGui.Run(connector);
	thUi.Run(userInterface);

	thUi.waitForFinish();
	thGui.waitForFinish();

	LogFileLogEndSession();
}

void* userInterface(void*) {
	NNTPCommand cmd;
	string response;
	semUserInterface.Wait();

	for (;;) {

		cmd.value = StrMng::Trim(Console::Read());
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se leyó de STDIN", NOPARMS);
		if(cmd.isEmpty())
			continue;
		if(cmd.isTooLong())
		{
			Console::Write(ERRORCODE);
			Console::Write(" ");
			Console::WriteLine(TOOLONGLINE);
			LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "El mensaje es demasiado largo", NOPARMS);
			continue;
		}
		shared.Write(cmd.getCommand());
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se escribió en SharedMemory", cmd.value.c_str());

		semConnector.Go();
		semUserInterface.Wait();

		response = shared.Read();
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se leyó de SharedMemory", response.c_str());
		Console::WriteLine(StrMng::CleanResponse(response));
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se escribió en STDOUT", response.c_str());

		if(cmd.isExit())
		{
			Finish = 1;
			semConnector.Go();
			break;
		}
	}
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Termino el Thread UserInterface", NOPARMS);
	return 0;
}

void* connector(void*) {
	string response;
	string command;

	xmlFile = LoadConfig(PathToConfig);
	string Port = XmlFileGetParam(xmlFile, "Port");
	xmlFile = LoadConfig(PathToConfig);
	string Host = XmlFileGetParam(xmlFile, "Host");
	FreeXmlFile(xmlFile);

	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se intenta Conectar a: ", NOPARMS);

    Console::Write("Trying ");
    Console::Write(Host);
    Console::WriteLine("...");

	SSLSocket socket(Host, atoi(Port.c_str()));

	Console::WriteLine("Connected to Open Source Team NNTP Server.");

	semUserInterface.Go();

	for (;;) {
		semConnector.Wait();

		if(Finish)
			break;

		command = shared.Read();
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se leyó de SharedMemory", command.c_str());
		if (socket.write(command) == SOCKETERROR)
		{
			Console::WriteLine("El servidor esta fuera de servicio");
			LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "El servidor esta fuera de servicio", command.c_str());
			LogFileLogEndSession();
			exit(1);
		}
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se escribió en el Socket", command.c_str());

		response = socket.read();
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se leyó del socket", response.c_str());
		shared.Write(response);
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se escribió en SharedMemory", response.c_str());

		semUserInterface.Go();

	}

	socket.Close();
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se cerro el Socket", NOPARMS);
	Console::WriteLine("Connection closed by foreign host.");
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Termino el Thread Connector", NOPARMS);
	return 0;
}
