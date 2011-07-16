typedef struct _ThreadClass *Thread, ThreadClass;

Thread NewThread(void);
void ThreadWaitForFinish(Thread thread);
void ThreadDestroy(Thread thread);
void ThreadRun(Thread thread, unsigned (__stdcall * target)(void*), int arg);
void ThreadEnd(Thread thread);
int ThreadIsAlive(Thread thread);
