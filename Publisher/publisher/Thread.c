#ifndef Thread_C
#define Thread_C

#include <windows.h>
#include "Thread.h"

#define MEGABYTE 1024*1024

Thread NewThread(void)
{	 
	HANDLE ThreadHeap = HeapCreate(0, MEGABYTE, 0);
    Thread thread = (Thread) HeapAlloc(ThreadHeap, HEAP_ZERO_MEMORY, sizeof(ThreadClass));	
	thread->THREADSTACK = MEGABYTE;
	thread->ThreadHeap = ThreadHeap;
	return thread;
}

void ThreadWaitForFinish(Thread thread)
{    
	WaitForSingleObject(thread->handler, INFINITE);
}

void ThreadDestroy(Thread thread)
{
	CloseHandle(thread->handler);
	HeapDestroy(thread->ThreadHeap);
}

void ThreadRun(Thread thread, void (*target)(void*))
{
	thread->handler = _beginthreadex(NULL , thread->THREADSTACK, target, NULL ,0, &thread->id);
}

void ThreadEnd(Thread thread)
{
	_endthreadex(thread->id);
	CloseHandle(thread->handler);
	HeapDestroy(thread->ThreadHeap);
}

int ThreadIsAlive(Thread thread)
{
	DWORD code;
	GetExitCodeThread(thread->handler, &code);
	if (code == STILL_ACTIVE)
		return 1;
	return 0;
}

#endif