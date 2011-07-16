#ifndef IPC_RPCParser_CPP
#define IPC_RPCParser_CPP

#include <iostream>
#include <string>
#include "StrMng.cpp"
#include "Utils/Logs.h"

using namespace std;
#define MINLENGTH 22
#define HANDSHAKE "Publisher-HandShake"

class IPC_RPC
{
	public:
		string idDescriptor;
		string payloadDescriptor;
		string payloadLength;
		string message;
};

class IPC_RPCParser
{
	private:
		IPC_RPC ipc_rpc;
	public:
		IPC_RPCParser()		
		{
		}
		IPC_RPC ParseMesage(string RawMessage)
		{
			ipc_rpc.idDescriptor = StrMng::SubString(RawMessage, 0, 16);
			ipc_rpc.payloadDescriptor = StrMng::SubString(RawMessage, 16, 18);
			ipc_rpc.payloadLength = StrMng::SubString(RawMessage, 18, 21);			
			ipc_rpc.message = StrMng::SubString(RawMessage, 21, RawMessage.length());
			LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se Parseo el ipc_rpc", NOPARMS);
			return ipc_rpc;
		}
		int isValid(string RawMessage)
		{
			if (RawMessage.length() < MINLENGTH)
				return 0;
			return 1;
		}
		int AutenticateHandShake(IPC_RPC ipc_rpc)
		{
			string hs = "Publisher-Nntp-HandShake";
			if (ipc_rpc.message == hs) {
				return 1;
			}
			return 0;
		}
};

#endif
