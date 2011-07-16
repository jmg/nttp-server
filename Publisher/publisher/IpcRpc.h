#define IPCRPCOK "00"
#define IPCRPCFAIL "01"

#define IDDESCRIPTORLEN 16
#define PAYLOADDESCRIPTORLEN 2
#define PAYLOADLEANGTHLEN 3

typedef struct _IPC_RPCClass
{
   char* idDescriptor; 
   char* payloadDescriptor; 
   char* payloadLength; 
   char* message;

} IPC_RPCClass, *IPC_RPC;

IPC_RPC newIPC_RPC(void);
void FreeIPC_RPC(IPC_RPC ipc_rpc);
char* IPC_RPCSerialize(IPC_RPC ipc_rpc);
IPC_RPC IPC_RPCPrepareMessage(char* xmlNew);	
IPC_RPC IPC_RPCPrepareHandShake();