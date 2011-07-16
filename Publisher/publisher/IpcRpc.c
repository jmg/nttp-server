#ifndef IpcRpc_C
#define IpcRpc_C

#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "IpcRpc.h"
#include "StrMng.h"
#include "Utils/Logs.h"

#define HANDSHAKE "Publisher-Nntp-HandShake"

char* DescriptorsGenerator(char* DescriptorId)
{
	int i;
	srand(time(0));
	for(i=0;i<16;i++)
		DescriptorId[i] = (rand() % 25) + 65;
	return DescriptorId;
}


IPC_RPC newIPC_RPC(int MessageLen)
{
	IPC_RPC ipc_rpc = (IPC_RPC)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(IPC_RPCClass));	
	ipc_rpc->idDescriptor = MemReserveForChar(IDDESCRIPTORLEN);	
	DescriptorsGenerator(ipc_rpc->idDescriptor);
	ipc_rpc->message = MemReserveForChar(MessageLen);	
	ipc_rpc->payloadDescriptor = MemReserveForChar(PAYLOADDESCRIPTORLEN);
	ipc_rpc->payloadLength = MemReserveForChar(PAYLOADLEANGTHLEN);	
	return ipc_rpc;
}

void FreeIPC_RPC(IPC_RPC ipc_rpc)
{
	HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, ipc_rpc);
}

IPC_RPC IPC_RPCPrepareMessage(char* xmlNew)
{
	int payLoadLen;
	IPC_RPC ipc_rpc = newIPC_RPC(strlen(xmlNew)+ PAYLOADLEANGTHLEN + PAYLOADDESCRIPTORLEN + IDDESCRIPTORLEN);
	strcpy(ipc_rpc->payloadDescriptor, IPCRPCOK);
	payLoadLen = strlen(xmlNew);
	if (payLoadLen < 999)
		sprintf(ipc_rpc->payloadLength, "%d", payLoadLen);
	else
		sprintf(ipc_rpc->payloadLength, "%d", 999);
	strcpy(ipc_rpc->message, xmlNew);
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se preparó el siguente mensaje", ipc_rpc->message);
	return ipc_rpc;
}

int IPC_RPCGetSize(IPC_RPC ipc_rpc)
{
	return strlen(ipc_rpc->message) + 2 + 3 + 16;
}

char* IPC_RPCSerialize(IPC_RPC ipc_rpc)
{
	char* ipc_rpcSerialized = MemReserveForChar(IPC_RPCGetSize(ipc_rpc));	
	strcat(ipc_rpcSerialized, ipc_rpc->idDescriptor);
	strcat(ipc_rpcSerialized, ipc_rpc->payloadDescriptor);
	strcat(ipc_rpcSerialized, ipc_rpc->payloadLength);
	strcat(ipc_rpcSerialized, ipc_rpc->message);
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se serializo el ipc_rpc: ", ipc_rpcSerialized);
	return ipc_rpcSerialized;
}



IPC_RPC IPC_RPCPrepareHandShake()
{
	int payLoadLen;	
	IPC_RPC ipc_rpc = newIPC_RPC(strlen(HANDSHAKE) + PAYLOADLEANGTHLEN + PAYLOADDESCRIPTORLEN + IDDESCRIPTORLEN);
	payLoadLen = strlen(HANDSHAKE);
	strcpy(ipc_rpc->message, HANDSHAKE);
	strcpy(ipc_rpc->payloadDescriptor, IPCRPCOK);	
	sprintf(ipc_rpc->payloadLength, "%d", payLoadLen);	
	strcat(ipc_rpc->payloadLength, "0");
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se preparó el handshake", ipc_rpc->message);
	return ipc_rpc;
}


#endif