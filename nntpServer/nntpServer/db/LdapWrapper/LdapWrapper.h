#ifndef LDAP_WRAPPER_H
#define LDAP_WRAPPER_H
#include <errno.h>
#include <stdarg.h>
#include <winldap.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifndef CONST
#define CONST               const
#endif

typedef unsigned long ULONG;
typedef ULONG *PULONG;
typedef unsigned short USHORT;
typedef USHORT *PUSHORT;
typedef unsigned char UCHAR;
typedef UCHAR *PUCHAR;
typedef char *PSZ;
typedef unsigned long DWORD;
typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef float FLOAT;
typedef FLOAT *PFLOAT;
typedef void VOID, *PVOID;
typedef char CHAR, *PCHAR;
typedef int INT;
typedef unsigned int UINT;
typedef unsigned int *PUINT;
typedef short SHORT;
typedef long LONG;
typedef double LONGLONG;
typedef double ULONGLONG;
typedef BYTE BOOLEAN;
typedef char **PPCHAR;
typedef LDAP *PLDAP;
typedef LDAPMessage *PLDAPMessage;

/**
 * Objeto que representa una contexto
 * de ejecucion LDAP
 *
 */
typedef struct __LDAP_CONTEXT {

	PCHAR host;
	INT errorCode;
	PLDAP ldap;

} LDAP_CONTEXT, *PLDAP_CONTEXT;

/**
 * Objeto que representa una sesion LDAP
 *
 */
typedef struct __LDAP_SESSION {

	PLDAP_CONTEXT context;
	PCHAR dn;
	PCHAR password;
	BOOLEAN started;
	INT errorCode;

} LDAP_SESSION, *PLDAP_SESSION;

/**
 * Attributo de una entry y operaciones sobre un
 * atributo
 *
 */
typedef struct __LDAP_ATTRIBUTE LDAP_ATTRIBUTE, *PLDAP_ATTRIBUTE;

struct __LDAP_ATTRIBUTE {

	LDAPMod *modObj;
	PLDAP_ATTRIBUTE next;

};

typedef PLDAP_ATTRIBUTE (* ATOP_CREATE_ATTRIB)(PCHAR, INT, ...);

typedef struct __LDAP_ATTRIBUTE_OP {

	ATOP_CREATE_ATTRIB createAttribute;

} LDAP_ATTRIBUTE_OP, *PLDAP_ATTRIBUTE_OP;

/**
 * Objeto que representa una entry
 * con valores y atributos
 */

typedef struct __LDAP_ENTRY_OBJ {

	PCHAR dn;
	LDAPMod **ldapMod;
	PLDAP_ATTRIBUTE firstAttribute;
	PLDAP_ATTRIBUTE lastAttribute;
	DWORD actualSize;

} LDAP_ENTRY, *PLDAP_ENTRY;

typedef PLDAP_ENTRY (* EOP_CREATE_ENTRY)();
typedef VOID (* EOP_ADD_PARAM)(PLDAP_ENTRY, PLDAP_ATTRIBUTE);
typedef VOID (* EOP_DEL_PARAM)(PLDAP_ENTRY, PLDAP_ATTRIBUTE);
typedef VOID (* EOP_MOD_PARAM)(PLDAP_ENTRY, PLDAP_ATTRIBUTE);

typedef struct __LDAP_ENTRY_OBJ_OPERATIONS {

	EOP_CREATE_ENTRY createEntry;
	EOP_ADD_PARAM addAttribute;
	EOP_DEL_PARAM deleteAttribute;
	EOP_MOD_PARAM editAttribute;

} LDAP_ENTRY_OP, *PLDAP_ENTRY_OP;

/**
 * Objetos implicados en busquedas. Iterador
 * sobre resultados.
 */
typedef struct __LDAP_RESULT_SET LDAP_RESULT_SET, *PLDAP_RESULT_SET;
typedef struct __LDAP_RECORD LDAP_RECORD, *PLDAP_RECORD;
typedef struct __LDAP_ITERATOR LDAP_ITERATOR, *PLDAP_ITERATOR;
typedef struct __LDAP_RECORD_OP LDAP_RECORD_OP, *PLDAP_RECORD_OP;
typedef struct __LDAP_FIELD LDAP_FIELD, *PLDAP_FIELD;

typedef BOOLEAN (* IOP_HAS_NEXT)(PLDAP_RESULT_SET);
typedef PLDAP_RECORD (* IOP_NEXT_ENTRY)(PLDAP_RESULT_SET);
typedef BOOLEAN (* RCOP_FIELD_HAS_NEXT)(PLDAP_RECORD);
typedef PLDAP_FIELD (*RCOP_FIELD_NEXT)(PLDAP_RECORD);

struct __LDAP_FIELD {

	PCHAR name;
	DWORD valuesSize;
	PPCHAR values;
};

struct __LDAP_RECORD {

	PLDAP ldap;
	PCHAR dn;
	PLDAPMessage message;
	BerElement *ber;
	PCHAR next;
};

struct __LDAP_ITERATOR {

	IOP_HAS_NEXT hasNext;
	IOP_NEXT_ENTRY next;
};

struct __LDAP_RESULT_SET {

	PLDAPMessage results;
	PLDAPMessage next;
	PLDAP_SESSION session;
	PLDAP_ITERATOR iterator;
	DWORD count;
};

struct __LDAP_RECORD_OP {

	RCOP_FIELD_HAS_NEXT hasNextField;
	RCOP_FIELD_NEXT nextField;
};

/**
 * Operaciones definidas sobre una session
 * LDAP.
 *
 */
typedef VOID (* SOP_START_SESSION)(PLDAP_SESSION);
typedef VOID (* SOP_END_SESSION)(PLDAP_SESSION);
typedef INT (* SOP_IS_STARTED)(PLDAP_SESSION);
typedef VOID (* SOP_ADD_ENTRY)(PLDAP_SESSION, PLDAP_ENTRY);
typedef VOID (* SOP_DEL_ENTRY_DN)(PLDAP_SESSION, PCHAR);
typedef VOID (* SOP_DEL_ENTRY_OBJ)(PLDAP_SESSION, PLDAP_ENTRY);
typedef VOID (* SOP_EDIT_ENTRY)(PLDAP_SESSION, PLDAP_ENTRY);
typedef PLDAP_RESULT_SET (* SOP_SEARCH_ENTRY)(PLDAP_SESSION, PCHAR, PCHAR);
typedef VOID (* SOP_ASYN_SEARCH_ENTRY)(PLDAP_SESSION, PCHAR, PCHAR);

typedef struct __LDAP_SESSION_OPERATIONS {

	SOP_START_SESSION startSession;
	SOP_END_SESSION endSession;
	SOP_IS_STARTED isStarted;
	SOP_ADD_ENTRY addEntry;
	SOP_DEL_ENTRY_DN deleteEntryDn;
	SOP_DEL_ENTRY_OBJ deleteEntryObj;
	SOP_EDIT_ENTRY editEntry;
	SOP_SEARCH_ENTRY searchEntry;
	SOP_ASYN_SEARCH_ENTRY searchEntryAsync;

} LDAP_SESSION_OP, *PLDAP_SESSION_OP;

/**
 * Operaciones definidas sobre una contexto
 * LDAP.
 *
 */
typedef VOID (* COP_SET_OPTION)(PLDAP_CONTEXT, INT, PVOID);
typedef PVOID (* COP_GET_OPTION)(PLDAP_CONTEXT, INT);
typedef VOID (* COP_INIT_CTX)(PLDAP_CONTEXT, PCHAR);
typedef PLDAP_SESSION (* COP_NEW_SESSION)(PLDAP_CONTEXT, PCHAR, PCHAR);

typedef struct __LDAP_CONTEXT_OPERATIONS {

	COP_SET_OPTION setOption;
	COP_GET_OPTION getOption;
	COP_INIT_CTX initialize;
	COP_NEW_SESSION newSession;

} LDAP_CONTEXT_OP, *PLDAP_CONTEXT_OP;

/**
 * Construtores para objetos y objetos con
 * operaciones
 */

PLDAP_CONTEXT newLDAPContext();

PLDAP_SESSION newLDAPSession();

PLDAP_CONTEXT_OP newLDAPContextOperations();

PLDAP_SESSION_OP newLDAPSessionOperations();

PLDAP_ENTRY_OP newLDAPEntryOperations();

PLDAP_ATTRIBUTE_OP newLDAPAttributeOperations();

PLDAP_ITERATOR newLDAPIterator();

PLDAP_RECORD_OP newLDAPRecordOperations();

/**
 * Destructores de objetos
 */

VOID freeLDAPContextOperations(PLDAP_CONTEXT_OP);

VOID freeLDAPSessionOperations(PLDAP_SESSION_OP);

VOID freeLDAPContext(PLDAP_CONTEXT);

VOID freeLDAPSession(PLDAP_SESSION);

VOID freeLDAPIterator(PLDAP_ITERATOR);

VOID freeLDAPRecordOperations(PLDAP_RECORD_OP);

VOID freeLDAPRecord(PLDAP_RECORD);

VOID freeLDAPField(PLDAP_FIELD);


#endif
