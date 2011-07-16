#ifndef RANDOM_CPP
#define RANDOM_CPP
#include <time.h>
#include <cstdlib>

class Random {

public:
	/* Inicializa la semilla del random con la hora del sistema */
	Random() {
		srand((unsigned) time(0));
	}
	/* Genera un random */
	int static Gen() {
		return rand();
	}
};

#endif
