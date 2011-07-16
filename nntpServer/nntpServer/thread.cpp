#ifndef thread_CPP
#define thread_CPP

#include "thread.h"
#include <windows.h>
#include <process.h>

struct _ThreadClass
{
	int THREADSTACK;
    unsigned int id;
	HANDLE ThreadHeap;
	HANDLE handler;

};

Thread NewThread(void)
{	 
	HANDLE ThreadHeap = HeapCreate(0, 0, 1024);
    Thread thread = (Thread) HeapAlloc(ThreadHeap, HEAP_ZERO_MEMORY, sizeof(ThreadClass));	
	thread->THREADSTACK = 1024;
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

void ThreadRun(Thread thread, unsigned (__stdcall * target)(void*), int arg)
{
	thread->handler = (HANDLE)_beginthreadex(NULL , thread->THREADSTACK, target, (void *)arg ,0, &thread->id);
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