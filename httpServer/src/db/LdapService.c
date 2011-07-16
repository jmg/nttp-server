#include "LdapService.h"
#define MAX_GROUPNAME_NAME = 500;

char * LdapServiceGetQuery(char * field, char * value);
AbstractNotice LdapServiceGetNotice(PLDAP_RECORD_OP recordOp,
		PLDAP_RECORD record);

struct ConcreteLdapService {
	char * serverUrl;
	PLDAP_CONTEXT context;
	PLDAP_CONTEXT_OP ctxOp;
	PLDAP_SESSION_OP sessionOp;
	PLDAP_ENTRY_OP entryOp;
	PLDAP_ATTRIBUTE_OP attribOp;
};

AbstractLdapService NewLdapService(char * host) {

	AbstractLdapService abstractLdapService;
	if ((abstractLdapService = malloc(sizeof(struct ConcreteLdapService)))) {
		abstractLdapService->serverUrl = host;
		/*FIXME(daniel) : Ver porq no funca si inicializamos aca.*/
	}
	return abstractLdapService;
}

void initializeContext(AbstractLdapService abstractLdapService) {
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se inicializa el contexto de LDAP. ", NOPARMS);

	abstractLdapService->context = newLDAPContext();
	abstractLdapService->ctxOp = newLDAPContextOperations();
	abstractLdapService->sessionOp = newLDAPSessionOperations();
	abstractLdapService->entryOp = newLDAPEntryOperations();
	abstractLdapService->attribOp = newLDAPAttributeOperations();
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se inicia el contexto de ejecucion de ldap en el host especificado", abstractLdapService->serverUrl);
		abstractLdapService->ctxOp->initialize(abstractLdapService->context,
			abstractLdapService->serverUrl);
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se termina de inicializar el contexto de LDAP. ", NOPARMS);
}

void closeContext(AbstractLdapService abstractLdapService) {
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se comienza a cerrar el contexto de LDAP. ", NOPARMS);
	freeLDAPContext(abstractLdapService->context);
	freeLDAPContextOperations(abstractLdapService->ctxOp);
	freeLDAPSessionOperations(abstractLdapService->sessionOp);
	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se termina de cerrar el contexto de LDAP. ", NOPARMS);
}

PLDAP_SESSION getSession(AbstractLdapService abstractLdapService) {

	PLDAP_SESSION session = abstractLdapService->ctxOp->newSession(
			abstractLdapService->context, "cn=Directory Manager", "password");
	abstractLdapService->sessionOp->startSession(session);

	return session;
}

void closeSession(AbstractLdapService abstractLdapService,
		PLDAP_SESSION session) {
	abstractLdapService->sessionOp->endSession(session);
	freeLDAPSession(session);
}

/*
 Esta funcion muestra como insertar una nueva entry
 con nuevos atributos y valores
 */
AbstractNotice LdapServiceGetNoticeById(
		AbstractLdapService abstractLdapService, char * id) {

	AbstractNotice notice;

	initializeContext(abstractLdapService);
	PLDAP_SESSION session = getSession(abstractLdapService);

	char * query = LdapServiceGetQuery("utnArticleID", id);

	/* Hacemos una consulta en una determinada rama aplicando la siguiente condicion */
	PLDAP_RESULT_SET resultSet = abstractLdapService->sessionOp->searchEntry(
			session, "ou=so,dn=utn,dn=edu", query);

	free(query);

	/*No se encontro la noticia!*/
	if(resultSet == NULL)
	{
		/* libero los recursos */
		closeSession(abstractLdapService, session);
		closeContext(abstractLdapService);
		return NULL;
	}

	/*No se encontro la noticia!*/
	if(resultSet->count <= 1)
	{
		/* libero los recursos */
		closeSession(abstractLdapService, session);
		closeContext(abstractLdapService);
		return NULL;
	}

	PLDAP_ITERATOR iterator = NULL;
	PLDAP_RECORD_OP recordOp = newLDAPRecordOperations();

	/* itero sobre los registros obtenidos
	  a traves de un iterador que conoce
	  la implementacion del recorset */
	for (iterator = resultSet->iterator; iterator->hasNext(resultSet);) {

		PLDAP_RECORD record = iterator->next(resultSet);

		notice = LdapServiceGetNotice(recordOp, record);

		/* libero los recursos consumidos
		  por el record */
		freeLDAPRecord(record);

	}

	/* libero los recursos */
	freeLDAPIterator(iterator);
	freeLDAPRecordOperations(recordOp);

	closeSession(abstractLdapService, session);
	closeContext(abstractLdapService);

	return notice;
}

AbstractNoticeGroups LdapServiceGetNoticesGroups(
		AbstractLdapService abstractLdapService) {

	initializeContext(abstractLdapService);

	PLDAP_SESSION session = getSession(abstractLdapService);

	char * query = LdapServiceGetQuery("utnArticleID", "*");
	/* Hacemos una consulta en una determinada rºama aplicando la siguiente condicion */
	PLDAP_RESULT_SET resultSet = abstractLdapService->sessionOp->searchEntry(
			session, "ou=so,dn=utn,dn=edu", query);
	free(query);

	/*can't contact ldap*/
	if(resultSet == NULL)
	{
		/* libero los recursos */
		closeSession(abstractLdapService, session);
		closeContext(abstractLdapService);
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Can't Contact Ldap", NOPARMS);
		return NULL;
	}

	/*No se encontro el grupo!*/
	if(resultSet->count <= 1)
	{
		/* libero los recursos */
		closeSession(abstractLdapService, session);
		closeContext(abstractLdapService);
		return NULL;
	}

	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "LdapService: Se busca todos los group names: ", NOPARMS);

	AbstractNoticeGroups noticeGroups = NewNoticeGroups();

	PLDAP_ITERATOR iterator = NULL;
	PLDAP_RECORD_OP recordOp = newLDAPRecordOperations();

	/* itero sobre los registros obtenidos
	  a traves de un iterador que conoce
	  la implementacion del recorset */
	for (iterator = resultSet->iterator; iterator->hasNext(resultSet);) {

		PLDAP_RECORD record = iterator->next(resultSet);

		AbstractNotice notice = LdapServiceGetNotice(recordOp, record);
		char * fieldValue = getGroupName(notice);

		/*Lo busco en el vector, si no esta, lo agrego*/
		int var = 0;
		BYTE encontrado;
		encontrado = 0;

		for (; var < NoticeGroupGetGroupsCount(noticeGroups); ++var) {

			if (strcmp(NoticeGroupGetGroupName(noticeGroups, var), fieldValue)
					== 0) {
				encontrado = 1;
				break;
			}
		}

		if (!encontrado) {
			NoticeGroupAddNoticeGroup(noticeGroups, fieldValue);
		}

		NoticeFree(notice);

		/* libero los recursos consumidos
		  por el record */
		freeLDAPRecord(record);
	}

	/* libero los recursos */
	freeLDAPIterator(iterator);
	freeLDAPRecordOperations(recordOp);
	free(resultSet);

	closeSession(abstractLdapService, session);
	closeContext(abstractLdapService);

	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "LdapService: Se termino de buscar todos los group names: ", NOPARMS);

	return noticeGroups;
}

AbstractStack LdapServiceGetNoticesByGroupName(
		AbstractLdapService abstractLdapService, char * groupName) {

	initializeContext(abstractLdapService);
	PLDAP_SESSION session = getSession(abstractLdapService);

	char * query = LdapServiceGetQuery("utnArticleGroupName", groupName);
	/* Hacemos una consulta en una determinada rama aplicando la siguiente condicion */
	PLDAP_RESULT_SET resultSet = abstractLdapService->sessionOp->searchEntry(
			session, "ou=so,dn=utn,dn=edu", query);
	free(query);

	/*can't contact ldap*/
	if(resultSet == NULL)
	{
		/* libero los recursos */
		closeSession(abstractLdapService, session);
		closeContext(abstractLdapService);
		LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEERROR, "Can't Contact Ldap", NOPARMS);
		return NULL;
	}

	/*No se encontro el grupo!*/
	if(resultSet->count <= 1)
	{
		/* libero los recursos */
		closeSession(abstractLdapService, session);
		closeContext(abstractLdapService);
		return NULL;
	}

	PLDAP_ITERATOR iterator = NULL;
	PLDAP_RECORD_OP recordOp = newLDAPRecordOperations();

	AbstractStack stackOfNotices = NewStack();

	for (iterator = resultSet->iterator; iterator->hasNext(resultSet);) {

		PLDAP_RECORD record = iterator->next(resultSet);

		StackPush(stackOfNotices, LdapServiceGetNotice(recordOp, record));

		/* libero los recursos consumidos
		  por el record */
		freeLDAPRecord(record);
	}

	/* libero los recursos */
	freeLDAPIterator(iterator);
	freeLDAPRecordOperations(recordOp);
	free(resultSet);

	closeSession(abstractLdapService, session);
	closeContext(abstractLdapService);

	return stackOfNotices;
}

char * LdapServiceGetQuery(char * field, char * value) {
	char * querySeparator = "=";

	int bufferSize = strlen(value) + strlen(field) + strlen(
			querySeparator);
	char * query = calloc(bufferSize + 5, sizeof(char));

	strcat(query, field);
	strcat(query, querySeparator);
	strcat(query, value);

	LogFileLogEntry(MAINPROCESSNAME, NOTHREADID, LOGTYPEINFO, "Se prepara la query LDAP siguiente ", query);
	return query;
}

AbstractNotice LdapServiceGetNotice(PLDAP_RECORD_OP recordOp,
		PLDAP_RECORD record) {
	char * id = calloc(30000, sizeof(char));
	char * body = calloc(30000, sizeof(char));
	char * header = calloc(30000, sizeof(char));
	char * groupName = calloc(30000, sizeof(char));

	/* Itero sobre los campos de cada
	  uno de los record */
	while (recordOp->hasNextField(record)) {
		PLDAP_FIELD field = recordOp->nextField(record);

		char * fieldValue = field->values[0];
		if (strcmp(field->name, "utnArticleID") == 0) {
			strcpy(id, fieldValue);
		} else if (strcmp(field->name, "utnArticleHead") == 0) {
			strcpy(header, fieldValue);
		} else if (strcmp(field->name, "utnArticleBody") == 0) {
			strcpy(body, fieldValue);
		} else if (strcmp(field->name, "utnArticleGroupName") == 0) {
			strcpy(groupName, fieldValue);
		}

		/* se libera la memoria utilizada
		  por el field si este ya no es
		  necesario. */
		freeLDAPField(field);
	}

	return NewNotice(id, header, body, groupName);
}
