#include "LdapWrapper.h"

#define __LDAP_ALLOC(POINTER_TO, STR_LENGTH, SIZE) \
			(POINTER_TO) malloc( sizeof(STR_LENGTH) * SIZE)

/*************************************************************************
 * Global LDAP utility functions
 *************************************************************************/

/**
 *
 */
PCHAR getErrorDescription(INT errorCode) {

	return ldap_err2string(errorCode);

}

VOID buildLDAPMod(PLDAP_ENTRY object) {

	object->ldapMod
			= __LDAP_ALLOC(LDAPMod **, LDAPMod *, object->actualSize + 1);

	PLDAP_ATTRIBUTE currentAttrib = object->firstAttribute;
	INT index = 0;

	while (currentAttrib != NULL) {

		object->ldapMod[index] = currentAttrib->modObj;

		currentAttrib = currentAttrib->next;

		index++;
	}

	object->ldapMod[index] = NULL;
}

/*************************************************************************
 * Definition of LDAP Context operations
 *************************************************************************/

/**
 *
 */
VOID setOption(PLDAP_CONTEXT context, INT option, PVOID value) {

	INT returnValue = ldap_set_option(context->ldap, option, value);

	if (returnValue != LDAP_OPT_SUCCESS) {

		context->errorCode = returnValue;
	}
}

/**
 *
 */
PVOID getOption(PLDAP_CONTEXT context, INT option) {

	PVOID value = NULL;
	INT returnValue = ldap_get_option(context->ldap, option, value);

	if (returnValue != LDAP_OPT_SUCCESS) {

		context->errorCode = returnValue;
		return NULL;
	}

	return value;
}

/**
 *
 */
VOID initialize(PLDAP_CONTEXT context, PCHAR host) {

	LDAP *ldap = NULL;
	INT returnValue;
	INT version = LDAP_VERSION3;

	returnValue = ldap_initialize(&ldap, host);

	if (returnValue != LDAP_SUCCESS) {

		printf("Init Error: %s\n", getErrorDescription(returnValue));
		context->errorCode = returnValue;
		return;
	}

	ldap_set_option(NULL, LDAP_OPT_PROTOCOL_VERSION, &version);

	context->ldap = ldap;
	context->host = host;
}

/**
 *
 */
PLDAP_SESSION newSession(PLDAP_CONTEXT context, PCHAR dn, PCHAR password) {

	PLDAP_SESSION session = newLDAPSession();

	if (context == NULL) {

		return NULL;
	}

	session->context = context;
	session->dn = dn;
	session->password = password;

	return session;
}

/*************************************************************************
 * Definition of LDAP Session operations
 *************************************************************************/

/**
 *
 */
VOID startSession(PLDAP_SESSION session) {

	PLDAP_CONTEXT context = session->context;

	ldap_simple_bind_s(context->ldap, session->dn, session->password);

	session->started = 1;

}

/**
 *
 */
VOID endSession(PLDAP_SESSION session) {

	PLDAP_CONTEXT context = session->context;

	ldap_unbind_s(context->ldap);

	session->started = 0;

}

/**
 *
 */
INT isStarted(PLDAP_SESSION session) {

	return session->started;

}

/**
 *
 */
VOID addEntry(PLDAP_SESSION session, PLDAP_ENTRY object) {

	buildLDAPMod(object);

	INT returnValue = ldap_add_ext_s(session->context->ldap, object->dn,
			object->ldapMod, NULL, NULL);

	if (returnValue != LDAP_OPT_SUCCESS) {

		session->errorCode = returnValue;
		printf("add Error: %s\n", getErrorDescription(returnValue));
	}

}

/**
 *
 */
INT doDeleteEntry(LDAP *ldap, PCHAR dn) {

	return ldap_delete_ext_s(ldap, dn, NULL, NULL);

}

/**
 *
 */
VOID deleteEntryObj(PLDAP_SESSION session, PLDAP_ENTRY entry) {

	INT returnValue = doDeleteEntry(session->context->ldap, entry->dn);

	if (returnValue != LDAP_OPT_SUCCESS) {

		session->errorCode = returnValue;
		printf("add Error: %s\n", getErrorDescription(returnValue));
	}

}

/**
 *
 */
VOID deleteEntryDn(PLDAP_SESSION session, PCHAR dn) {

	INT returnValue = doDeleteEntry(session->context->ldap, dn);

	if (returnValue != LDAP_OPT_SUCCESS) {

		session->errorCode = returnValue;
		printf("add Error: %s\n", getErrorDescription(returnValue));
	}

}

VOID editEntry(PLDAP_SESSION session, PLDAP_ENTRY entry) {

	buildLDAPMod(entry);

	INT returnValue = ldap_modify_ext_s(session->context->ldap, entry->dn,
			entry->ldapMod, NULL, NULL);

	if (returnValue != LDAP_OPT_SUCCESS) {

		session->errorCode = returnValue;
		printf("add Error: %s\n", getErrorDescription(returnValue));
	}

}

/**
 *
 */
PLDAP_RESULT_SET searchEntry(PLDAP_SESSION session, PCHAR baseDn, PCHAR filter) {

	PLDAP_RESULT_SET resultSet =
			__LDAP_ALLOC(PLDAP_RESULT_SET, LDAP_RESULT_SET, 1);
	PLDAP_ITERATOR iterator = newLDAPIterator();

	INT returnValue = ldap_search_ext_s(session->context->ldap, baseDn,
			LDAP_SCOPE_SUBTREE, filter, NULL, 0, NULL, NULL, LDAP_NO_LIMIT,
			LDAP_NO_LIMIT, &resultSet->results);

	if (returnValue != LDAP_OPT_SUCCESS) {

		session->errorCode = returnValue;
		printf("add Error: %s\n", getErrorDescription(returnValue));
		return NULL;
	}

	/* guardo la cantidad de resultados obtenidos */
	resultSet->count = ldap_count_messages(session->context->ldap,
			resultSet->results);
	resultSet->iterator = iterator;
	resultSet->session = session;

	/* guardo el primer mensaje de los
	 resultador para el iterador */
	resultSet->next = ldap_first_entry(session->context->ldap,
			resultSet->results);

	return resultSet;

}

VOID searchEntryAsync(PLDAP_SESSION session, PCHAR baseDn, PCHAR filter) {

	INT msgid;

	INT returnValue = ldap_search_ext(session->context->ldap, baseDn,
			LDAP_SCOPE_SUBTREE, filter, NULL, 0, NULL, NULL, LDAP_NO_LIMIT,
			LDAP_NO_LIMIT, &msgid);

	if (returnValue != LDAP_OPT_SUCCESS) {

		session->errorCode = returnValue;
		printf("add Error: %s\n", getErrorDescription(returnValue));
		return;
	}
}

/*************************************************************************
 * Entries LDAP functions
 *************************************************************************/

/**
 *Init
 */
PLDAP_ATTRIBUTE createAttribute(PCHAR attributeName, INT valueSize, ...) {

	PLDAP_ATTRIBUTE attribute = NULL;
	LDAPMod *modObj = NULL;
	PPCHAR attributes = NULL;
	UINT index;
	va_list argPtr;

	attribute = __LDAP_ALLOC(PLDAP_ATTRIBUTE, LDAP_ATTRIBUTE, 1);
	modObj = __LDAP_ALLOC(LDAPMod *, LDAPMod, 1);
	attributes = __LDAP_ALLOC(PPCHAR, PCHAR, valueSize + 1);

	va_start(argPtr, valueSize);

	/* extraigo los argumentos de la lista
	 y genero el array de attributes */
	for (index = 0; index < valueSize; index++) {

		attributes[index] = va_arg(argPtr, PCHAR);

	}

	/* se finaliza indicando con null
	 que no hay mas attributos */
	attributes[index] = NULL;

	va_end(argPtr);

	modObj->mod_type = attributeName;
	modObj->mod_values = attributes;

	attribute->modObj = modObj;
	attribute->next = NULL;

	return attribute;
}

/**
 *
 */
PLDAP_ENTRY createEntry() {

	PLDAP_ENTRY entry = __LDAP_ALLOC(PLDAP_ENTRY, LDAP_ENTRY, 1);

	entry->actualSize = 0;
	entry->firstAttribute = NULL;
	entry->lastAttribute = NULL;
	entry->ldapMod = NULL;

	return entry;
}

VOID appendAttribute(PLDAP_ENTRY entry, PLDAP_ATTRIBUTE attribute,
		INT operationType) {

	/* establesco el tipo de operacion */
	attribute->modObj->mod_op = operationType;

	/* si es el primer attribute */
	if (entry->firstAttribute == NULL) {

		entry->firstAttribute = attribute;
		entry->lastAttribute = attribute;
		entry->actualSize = 1;
		return;
	}

	/* lo agrego al final */
	entry->lastAttribute->next = attribute;
	entry->lastAttribute = attribute;
	entry->actualSize++;
}

VOID addAttribute(PLDAP_ENTRY entry, PLDAP_ATTRIBUTE attribute) {

	appendAttribute(entry, attribute, LDAP_MOD_ADD);

}

VOID deleteAttribute(PLDAP_ENTRY entry, PLDAP_ATTRIBUTE attribute) {

	appendAttribute(entry, attribute, LDAP_MOD_DELETE);

}

VOID editAttribute(PLDAP_ENTRY entry, PLDAP_ATTRIBUTE attribute) {

	appendAttribute(entry, attribute, LDAP_MOD_REPLACE);

}

/*************************************************************************
 * Iterator LDAP functions
 *************************************************************************/
BOOLEAN hasNext(PLDAP_RESULT_SET resultSet) {

	return (resultSet->next == NULL) ? FALSE : TRUE;

}

PLDAP_RECORD next(PLDAP_RESULT_SET resultSet) {

	PLDAP_RECORD record = __LDAP_ALLOC(PLDAP_RECORD, LDAP_RECORD, 1);

	if (resultSet->next == NULL) {

		return NULL;
	}

	record->ldap = resultSet->session->context->ldap;
	record->message = resultSet->next;
	record->dn = ldap_get_dn(record->ldap, resultSet->next);
	record->next = ldap_first_attribute(record->ldap, record->message,
			&record->ber);
	resultSet->next = ldap_next_entry(record->ldap, record->message);

	return record;
}

/*************************************************************************
 * Record LDAP functions
 *************************************************************************/

BOOLEAN hasNextField(PLDAP_RECORD record) {

	return (record->next == NULL) ? FALSE : TRUE;
}

PLDAP_FIELD nextField(PLDAP_RECORD record) {

	PLDAP_FIELD field = __LDAP_ALLOC(PLDAP_FIELD, LDAP_FIELD, 1);

	if (record->next == NULL) {

		return NULL;
	}

	field->name = record->next;
	field->values = (char **) ldap_get_values(record->ldap, record->message,
			record->next);
	field->valuesSize = ldap_count_values(field->values);
	record->next = ldap_next_attribute(record->ldap, record->message,
			record->ber);

	return field;

}

/*************************************************************************
 * Initialization LDAP functions
 *************************************************************************/

PLDAP_CONTEXT newLDAPContext() {

	PLDAP_CONTEXT context = __LDAP_ALLOC(PLDAP_CONTEXT, LDAP_CONTEXT, 1);

	context->errorCode = 0;
	context->host = NULL;
	context->ldap = NULL;

	return context;
}

PLDAP_SESSION newLDAPSession() {

	PLDAP_SESSION session = __LDAP_ALLOC(PLDAP_SESSION, LDAP_SESSION, 1);

	session->context = NULL;
	session->dn = NULL;
	session->errorCode = 0;
	session->password = NULL;
	session->started = 0;

	return session;
}

/**
 *
 */
PLDAP_CONTEXT_OP newLDAPContextOperations() {

	PLDAP_CONTEXT_OP operations =
			__LDAP_ALLOC(PLDAP_CONTEXT_OP, LDAP_CONTEXT_OP, 1);

	operations->newSession = newSession;
	operations->getOption = getOption;
	operations->setOption = setOption;
	operations->initialize = initialize;

	return operations;
}

/**
 *
 */
PLDAP_SESSION_OP newLDAPSessionOperations() {

	PLDAP_SESSION_OP operations =
			__LDAP_ALLOC(PLDAP_SESSION_OP, LDAP_SESSION_OP, 1);

	operations->addEntry = addEntry;
	operations->deleteEntryDn = deleteEntryDn;
	operations->deleteEntryObj = deleteEntryObj;
	operations->editEntry = editEntry;
	operations->searchEntry = searchEntry;
	operations->searchEntryAsync = searchEntryAsync;
	operations->startSession = startSession;
	operations->endSession = endSession;
	operations->isStarted = isStarted;

	return operations;
}

PLDAP_ENTRY_OP newLDAPEntryOperations() {

	PLDAP_ENTRY_OP operations = __LDAP_ALLOC(PLDAP_ENTRY_OP, LDAP_ENTRY_OP, 1);

	operations->addAttribute = addAttribute;
	operations->deleteAttribute = deleteAttribute;
	operations->editAttribute = editAttribute;
	operations->createEntry = createEntry;

	return operations;
}

PLDAP_ATTRIBUTE_OP newLDAPAttributeOperations() {

	PLDAP_ATTRIBUTE_OP operations =
			__LDAP_ALLOC(PLDAP_ATTRIBUTE_OP, LDAP_ATTRIBUTE_OP, 1);

	operations->createAttribute = createAttribute;

	return operations;

}

PLDAP_ITERATOR newLDAPIterator() {

	PLDAP_ITERATOR iterator = __LDAP_ALLOC(PLDAP_ITERATOR, LDAP_ITERATOR, 1);

	iterator->hasNext = hasNext;
	iterator->next = next;

	return iterator;
}

PLDAP_RECORD_OP newLDAPRecordOperations() {

	PLDAP_RECORD_OP operations =
			__LDAP_ALLOC(PLDAP_RECORD_OP, LDAP_RECORD_OP, 1);

	operations->nextField = nextField;
	operations->hasNextField = hasNextField;
	return operations;

}

/**
 *
 */
void freeLDAPContextOperations(PLDAP_CONTEXT_OP operations) {

	if (NULL == operations)
		return;

	free(operations);

}

/**
 *
 */
void freeLDAPSessionOperations(PLDAP_SESSION_OP operations) {

	if (NULL == operations)
		return;

	free(operations);
}

/**
 *
 */
void freeLDAPSession(PLDAP_SESSION session) {

	if (NULL == session)
		return;

	free(session);
}

/**
 *
 */
void freeLDAPContext(PLDAP_CONTEXT context) {

	if (NULL == context)
		return;

	free(context);
}

/**
 *
 */
VOID freeLDAPIterator(PLDAP_ITERATOR iterator) {

	if (NULL == iterator)
		return;

	free(iterator);

}

/**
 *
 */
VOID freeLDAPRecordOperations(PLDAP_RECORD_OP operations) {

	if (NULL == operations)
		return;

	free(operations);

}

/**
 *
 */
VOID freeLDAPRecord(PLDAP_RECORD record) {

	if (NULL == record)
		return;

	ber_free(record->ber, 0);
	ldap_memfree(record->dn);
	ldap_memfree(record->message);
	free(record);
}

/**
 *
 */
VOID freeLDAPField(PLDAP_FIELD field) {

	if (NULL == field)
		return;

	ldap_memfree(field->values);
	free(field);
}
