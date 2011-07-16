#include "MemCachedService.h"
#include <libmemcached/memcached.h>
#include <string.h>



struct ConcreteMemCachedService {
	memcached_st *memc;
	memcached_st *memc2;
};

AbstractMemCachedService NewMemCachedService(char * host, int port1, char* host2, int port2) {
	AbstractMemCachedService abstractMemCachedService;
	if ((abstractMemCachedService = malloc(sizeof(struct ConcreteMemCachedService)))) {
		abstractMemCachedService->memc = memcached_create(NULL);
		memcached_server_add(abstractMemCachedService->memc, host, port1);
		abstractMemCachedService->memc2 = memcached_create(NULL);
		memcached_server_add(abstractMemCachedService->memc2, host2, port2);
	}
	return abstractMemCachedService;
}

void liberarCache(AbstractMemCachedService abstractMemCachedService) {
	memcached_free(abstractMemCachedService->memc);
	memcached_free(abstractMemCachedService->memc2);
}

void setearCache(AbstractMemCachedService abstractMemCachedService, char* clave, char* valor) {
	memcached_set(abstractMemCachedService->memc, clave, strlen(clave), valor, strlen(valor), 0, 0);
	memcached_set(abstractMemCachedService->memc2, clave, strlen(clave), valor, strlen(valor), 0, 0);
}

char* getCache(AbstractMemCachedService abstractMemCachedService, char* clave) {
	memcached_return rc;
	char* result;
	size_t result_length = 0;
	uint32_t flags = 0;
	result = memcached_get(abstractMemCachedService->memc, clave, strlen(clave), &result_length, &flags, &rc);
	if (!result)
	{
		result = memcached_get(abstractMemCachedService->memc2, clave, strlen(clave), &result_length, &flags, &rc);
	}
	return result;
}

