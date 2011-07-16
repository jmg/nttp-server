#ifndef STACK_H
#define STACK_H
#include <stdio.h>
#include <stdlib.h>

/* Se oculta lo mejor posible los tipos de datos a usar.
 * C es tipado y dificulta el hiding.
 */
/* El siguiente tipo define el tipo de datos que manipula
 * el TAD Pila. Como se observa los elementos son direcciones
 * previamente reservadas por el usuario (heap, stack)
 * pero no pueden ser constantes
 */
typedef void* stackElementT;

/* El siguiente tipo define el TAD Pila.
 * Se lo define como puntero a "cierta estructura"
 * para ocultar la real
 * reprentacion de la misma
 */
typedef struct ConcreteStack * AbstractStack;

/* Operaciones con las que se puede acceder al TAD                                                                                                                   */
/* Funcion: inicializa la pila
 * Uso: pila= NewStack();
 * -----------------------------
 * Usarla una sola vez antes de empezar a usar el resto
 * de las operaciones. Si hay error en la creación, retorna NULL.
 */
AbstractStack NewStack(void);

/* Funcion: destruye la pila
 * Uso: FreeStack( pila );
 * -----------------------------
 * Usarla una sola vez al finalizar el uso de la pila
 * Despues no volver a invocar operaciones sobre la pila
 */
void StackFree(AbstractStack stack);

/* Funcion: coloca un elemento nuevo en el stack
 * Uso: double nro= 5; Push( pila, &nro);
 * -----------------------------------------------
 * Cada vez que se desee pushear un elemento en el stack
 * se debe pasar su dirección, previamente reservada por
 * el usuario.
 */
void StackPush(AbstractStack pila, stackElementT elemento);

/* Funcion: desapila el tope de la pila.
 * Uso: double *puntero;                                     puntero= Pop( pila);
 * ----------------------------------------------------
 * Usarla para obtener el elemento tope de la pila.
 * La misma devuelve la dirección de donde se encuentra
 * dicho valor de memoria o NULL si la pila está vacía
 */
stackElementT StackPop(AbstractStack pila);

/* Funcion: dice si está o no vacía
 * Uso: if ( StackIsEmpty( pila ) ...
 * devuelve 1 si la pila está vacía o cero en caso contrario
 */
int StackIsEmpty(AbstractStack pila);

/* Funcion: devuelve el tope de la pila sin retirarlo
 * Uso: double *puntero;                                     puntero= Tope( pila);
 * Usarla para obtener una referencia al tope de la pila sin
 * retirarlo. Devuleve la dirección de donde se encuentra
 * dicho valor de memoria o NULL si la pila está vacía
 */
stackElementT StackTope(AbstractStack pila);

int StackGetSize(AbstractStack stack);


#endif
