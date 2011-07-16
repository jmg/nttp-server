#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include "Thread.h"

struct ConcreteThread {
	int _Id;
	pthread_t _thread;
};

AbstractThread NewThread(void) {
	AbstractThread thread;
	if ((thread = malloc(sizeof(struct ConcreteThread))) != NULL) {
		/* Inicializa la semilla del random con la hora del sistema */
		srand((unsigned) time(0));
		/* Genera un random */
		thread->_Id = rand();
	}
	return thread;
}

void runWithParam(AbstractThread thread, void* ( target)(void *), void * param) {
	pthread_create(&(thread -> _thread), NULL, target, param);
}

void run(AbstractThread thread, void* ( target)(void *)) {
	runWithParam(thread, target, (void*) &(thread->_Id));
}

void waitForFinish(AbstractThread thread) {
	pthread_join((thread -> _thread), NULL);
}
