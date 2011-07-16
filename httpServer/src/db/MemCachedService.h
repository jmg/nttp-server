#ifndef MEMCACHEDSERVICE_H
#define MEMCACHEDSERVICE_H

typedef struct ConcreteMemCachedService * AbstractMemCachedService;

AbstractMemCachedService NewMemCachedService(char * host, int port1, char* host2, int port2);

void liberarCache(AbstractMemCachedService);

void setearCache(AbstractMemCachedService, char*, char*);

char* getCache(AbstractMemCachedService, char*);

#endif
