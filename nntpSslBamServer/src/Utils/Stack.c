#include "Stack.h"
#define DIM 5000

struct ConcreteStack {
	stackElementT datos[DIM];
	int cant;
};

AbstractStack NewStack(void) {
	AbstractStack pila;
	if ((pila = malloc(sizeof(struct ConcreteStack))) != NULL)
		pila->cant = 0;
	return pila;
}

void StackFree(AbstractStack pila) {
	free(pila);
}

void StackPush(AbstractStack pila, stackElementT elemento) {
	if (pila->cant < DIM)
		pila->datos[pila->cant++] = elemento;
}

stackElementT StackPop(AbstractStack pila) {
	if (!StackIsEmpty(pila))
		return pila->datos[--pila->cant];
	else
		return NULL;
}

stackElementT StackGetFirst(AbstractStack pila) {
	if (!StackIsEmpty(pila))
		return pila->datos[0];
	else
		return NULL;
}

int StackIsEmpty(AbstractStack pila) {
	return pila->cant == 0;
}

stackElementT StackTope(AbstractStack pila) {
	if (!StackIsEmpty(pila))
		return pila->datos[pila->cant - 1];
	else
		return NULL;
}

int StackGetSize(AbstractStack stack) {
	return stack->cant;
}
