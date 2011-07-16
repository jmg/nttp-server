#ifndef THREAD_H
#define THREAD_H

typedef struct ConcreteThread * AbstractThread;

AbstractThread NewThread(void);

void runWithParam(AbstractThread thread, void* ( target)(void *), void * param);

void run(AbstractThread thread, void* ( target)(void *));

void waitForFinish(AbstractThread thread);

#endif
