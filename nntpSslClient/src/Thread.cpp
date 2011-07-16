#ifndef THREAD_CPP
#define THREAD_CPP
#include <pthread.h>
#include "Random.cpp"

class Thread {
public:
	Thread() {
	}
	/* Corre el thread sobre una funcion pasada como argumento */
	void Run(void* ( target)(void *)) {
		_Id = Random::Gen();
		pthread_create(&_thread, NULL, target, (void*) &_Id);
	}
	/* Devuelve el id del objeto thread */
	int GetId(void) {
		return _Id;
	}
	/* Devuelve el struct pthread_t del objeto thread */
	pthread_t GetThread() {
		return _thread;
	}
	void Sleep(int seconds) {
		Sleep(seconds);
	}
	void waitForFinish() {
		pthread_join(_thread, NULL);
	}
private:
	int _Id;
	pthread_t _thread;
};
#endif
