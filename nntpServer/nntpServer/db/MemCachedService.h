#ifndef MEMCACHEDSERVICE_H
#define MEMCACHEDSERVICE_H

typedef struct ConcreteMemCachedService * AbstractMemCachedService;

AbstractMemCachedService NewMemCachedService(char * host);

void liberarCache(AbstractMemCachedService);

void setearCache(AbstractMemCachedService, char*, char*);

char* getCache(AbstractMemCachedService, char*);

#endif
