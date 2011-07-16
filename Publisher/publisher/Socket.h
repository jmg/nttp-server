typedef struct SocketClass *Socket;

Socket NewSocket(void);
int SocketConnect(Socket Sock, char* host, int port);
char* SocketRead(Socket Sock);
int SocketWrite(Socket Sock, char* messsage);
void close(Socket sock);
void FreeSocket(Socket Sock);