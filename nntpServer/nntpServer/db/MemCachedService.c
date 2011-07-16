#include "MemCachedService.h"
#include <libmemcached/memcached.h>
#include <string.h>

#define SERVER_PORT 11211
#define SERVER_PORT2 11212

struct ConcreteMemCachedService {
	memcached_st *memc;
};

AbstractMemCachedService NewMemCachedService(char * host) {
	AbstractMemCachedService abstractMemCachedService;
	if ((abstractMemCachedService = malloc(
			sizeof(struct ConcreteMemCachedService)))) {
		abstractMemCachedService->memc = memcached_create(NULL);
		memcached_server_add(abstractMemCachedService->memc, host, SERVER_PORT);
		memcached_server_add(abstractMemCachedService->memc, host, SERVER_PORT2);
	}
	return abstractMemCachedService;
}

void liberarCache(AbstractMemCachedService abstractMemCachedService) {
	memcached_free(abstractMemCachedService->memc);
}

void setearCache(AbstractMemCachedService abstractMemCachedService,
		char* clave, char* valor) {
	memcached_set(abstractMemCachedService->memc, clave, strlen(clave), valor,
			strlen(valor), 0, 0);
}

char* getCache(AbstractMemCachedService abstractMemCachedService, char* clave) {
	memcached_return rc;
	char* result;
	int result_length = 0;
	int flags = 0;
	result = memcached_get(abstractMemCachedService->memc, clave,
			strlen(clave), &result_length, &flags, &rc);
	return result;
}
