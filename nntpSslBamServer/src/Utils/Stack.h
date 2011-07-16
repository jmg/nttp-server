#ifndef STACK_H
#define STACK_H
#include <stdio.h>
#include <stdlib.h>

typedef void* stackElementT;
typedef struct ConcreteStack * AbstractStack;

AbstractStack NewStack(void);
void StackFree(AbstractStack stack);
void StackPush(AbstractStack pila, stackElementT elemento);
stackElementT StackPop(AbstractStack pila);
int StackIsEmpty(AbstractStack pila);
stackElementT StackTope(AbstractStack pila);
stackElementT StackGetFirst(AbstractStack pila);

int StackGetSize(AbstractStack stack);

#endif
