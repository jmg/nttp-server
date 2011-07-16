typedef struct _ThreadClass
{
	int THREADSTACK;
    DWORD id;
	HANDLE ThreadHeap;
	HANDLE handler;

} ThreadClass, *Thread;

void ThreadWaitForFinish(Thread thread);
Thread NewThread(void);
void ThreadRun(Thread thread, void (*target)(void*));
void ThreadEnd(Thread thread);
int ThreadIsAlive(Thread thread);
void ThreadDestroy(Thread thread);
