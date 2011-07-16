/*
 Api que actua de wrapper sobre la api original de openLDAP
 */

#include <stdio.h>
#include "LdapWrapper.h"

/*
 Esta funcion muestra como insertar una nueva entry
 con nuevos atributos y valores
 */
void insertEntry(PLDAP_SESSION session, PLDAP_SESSION_OP sessionOp,
		PLDAP_ENTRY_OP entryOp, PLDAP_ATTRIBUTE_OP attribOp) {

	/* Creo una nueva entry
	 y le agrego los parametros correspondientes */
	PLDAP_ENTRY entry = entryOp->createEntry();

	entry->dn = "utnArticleID=12345,ou=so,dn=utn,dn=edu";
	entryOp->addAttribute(entry, attribOp->createAttribute("objectclass", 2,
			"top", "utnArticle"));
	entryOp->addAttribute(entry, attribOp->createAttribute("utnArticleID", 1,
			"12345"));
	entryOp->addAttribute(entry, attribOp->createAttribute("utnArticleHead", 1,
			"Head del articulo"));
	entryOp->addAttribute(entry, attribOp->createAttribute("utnArticleBody", 1,
			"Body del articulo"));
	entryOp->addAttribute(entry, attribOp->createAttribute(
			"utnArticleGroupName", 1, "Group del articulo"));
	sessionOp->addEntry(session, entry);

}

/*
 Eliminamos una entrada existente a partir
 de un dn conocido.
 */
void delEntry(PLDAP_SESSION session, PLDAP_SESSION_OP sessionOp,
		PLDAP_ENTRY_OP entryOp, PLDAP_ATTRIBUTE_OP attribOp) {

	/* creo una nueva entry
	 y le agrego los parametros correspondientes */
	PLDAP_ENTRY entry = entryOp->createEntry();

	entry->dn = "utnArticleID=12345,ou=so,dn=utn,dn=edu";

	/* Se puede eliminar una entry pasando un objeto
	 entry como parametro */
	sessionOp->deleteEntryObj(session, entry);

	/* O se puede eliminar una entry pasando
	 el dn correspondiente */
	sessionOp->deleteEntryDn(session, "utnArticleID=12345,ou=so,dn=utn,dn=edu");
}

/*
 Modificamos el valor de un atributo en una entry.
 Es necesario conocer el dn.
 */
void modifyEntry(PLDAP_SESSION session, PLDAP_SESSION_OP sessionOp,
		PLDAP_ENTRY_OP entryOp, PLDAP_ATTRIBUTE_OP attribOp) {

	PLDAP_ENTRY entry = entryOp->createEntry();

	entry->dn = "utnArticleID=12345,ou=so,dn=utn,dn=edu";

	/* agregamos el atributo a la entry en modo delete */
	entryOp->editAttribute(entry, attribOp->createAttribute("utnurlTitle", 1,
			"Pagina de Prueba para LDAP"));

	sessionOp->editEntry(session, (PLDAP_ENTRY)"utnArticleID=12345,ou=so,dn=utn,dn=edu");

}

/**
 Realizamos una consulta al directorio en una determinada
 rama. Para iterar sobre los resultados se utiliza un
 patron Iterator que recorre cada una de las entries
 */
void selectEntries(PLDAP_SESSION session, PLDAP_SESSION_OP sessionOp,
		PLDAP_ENTRY_OP entryOp, PLDAP_ATTRIBUTE_OP attribOp) {

	/* Hacemos una consulta en una determinada rama aplicando la siguiente condicion */
	PLDAP_RESULT_SET resultSet = sessionOp->searchEntry(session,
			"ou=so,dn=utn,dn=edu", "utnArticleID=*");
	PLDAP_ITERATOR iterator = NULL;
	PLDAP_RECORD_OP recordOp = newLDAPRecordOperations();

	/* iteramos sobre los registros obtenidos a traves de un iterador que conoce
	 la implementacion del recordset */
	for (iterator = resultSet->iterator; iterator->hasNext(resultSet);) {

		PLDAP_RECORD record = iterator->next(resultSet);

		printf("dn: %s\n", record->dn);

		/* Iteramos sobre los campos de cada uno de los records */
		while (recordOp->hasNextField(record)) {

			PLDAP_FIELD field = recordOp->nextField(record);
			INT index = 0;

			printf("	attribute: %s - values: %l\n", field->name,
					field->valuesSize);

			for (; index < field->valuesSize; index++) {

				printf("			Value[%d]: %s\n", index, field->values[index]);

			}

			/* liberamos la memoria utilizada por el field si este ya no es
			 necesario. */
			freeLDAPField(field);

		}

		/* liberamos los recursos consumidos por el record */
		freeLDAPRecord(record);

	}

	/* liberamos los recursos */
	freeLDAPIterator(iterator);
	freeLDAPRecordOperations(recordOp);

}

