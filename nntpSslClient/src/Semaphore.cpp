#ifndef SEMAPHORE_CPP
#define SEMAPHORE_CPP
#include <semaphore.h>

class Semaphore {
public:
	/* Inicializa el semaforo en un estado inicial
	 - value <= 0 tiene luz roja
	 - value > 0 tiene luz verde */
	Semaphore(int InitValue) {
		sem_init(&sem, 0, InitValue);
	}
	/* decrementa el contador interno del semaforo,
	 luego de ello:
	 -Si el valor del contador es <= 0, espera por un Go()
	 -de lo contrario, tiene luz verde  */
	void Wait() {
		sem_wait(&sem);
	}
	/* incrementa el contador interno del semaforo */
	void Go() {
		sem_post(&sem);
	}
	void TryWait() {
		sem_trywait(&sem);
	}
	/* Obtiene el estado del semaforo */
	int GetState() {
		int value;
		sem_getvalue(&sem, &value);
		return value;
	}
private:
	sem_t sem;
};
#endif
