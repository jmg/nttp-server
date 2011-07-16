CONNECTION = ClientConnection.o HttpConnectionDriver.o
DB = DatabaseService.o LdapService.o MemCachedService.o
LDAP = LdapWrapper.o
SERVER = ServerSocket.o Thread.o   
UTILS = Config.o Stack.o Utils.o UrlParser.o Logs.o
NOTICE = Notice.o NoticeGroups.o

LIBS = -lpthread -lsocket -lnsl -lmemcached -lldap -llber
LIBDIR = /usr/local/lib
HTTPLIBS = -L$(LIBDIR) $(LIBS)

HttpServerSolaris:  $(NOTICE) $(CONNECTION) $(DB) $(LDAP) $(SERVER) $(UTILS)   
	gcc HttpServer.c $(NOTICE) $(CONNECTION) $(DB) $(LDAP) $(SERVER) $(UTILS) $(HTTPLIBS) -ansi	-o	httpserver  

HttpServerDebian:  $(NOTICE) $(CONNECTION) $(DB) $(LDAP) $(SERVER) $(UTILS)   
	gcc HttpServer.c $(NOTICE) $(CONNECTION) $(DB) $(LDAP) $(SERVER) $(UTILS) -lpthread -lmemcached -lldap -llber -ansi	-o	httpserver  

Notice.o:	./Notice.c	./Notice.h
	gcc -c ./Notice.c

NoticeGroups.o:		./NoticeGroups.c	./NoticeGroups.h
	gcc -c ./NoticeGroups.c

Config.o:	./utils/Config.c	./utils/Config.h
	gcc -c ./utils/Config.c
	
Stack.o:	./utils/Stack.c	./utils/Stack.h
	gcc -c ./utils/Stack.c
	
Utils.o:	./utils/Utils.c
	gcc -c ./utils/Utils.c

UrlParser.o: ./utils/UrlParser.c ./utils/UrlParser.h
	gcc -c ./utils/UrlParser.c

Logs.o: ./utils/Logs.c ./utils/Logs.h
	gcc -c ./utils/Logs.c

ServerSocket.o:	./server/ServerSocket.c	./server/ServerSocket.h
	gcc -c ./server/ServerSocket.c

Thread.o:	./server/Thread.c	./server/Thread.h
	gcc -c ./server/Thread.c
	
DatabaseService.o: ./db/DatabaseService.c	./db/DatabaseService.h
	gcc -c ./db/DatabaseService.c

LdapService.o:	./db/LdapService.c	./db/LdapService.h
	gcc -c ./db/LdapService.c

Ldap.o: ./db/LdapWrapper/Ldap.c ./db/LdapWrapper/LdapWrapper.h
	gcc -c ./db/LdapWrapper/Ldap.c

LdapWrapper.o: ./db/LdapWrapper/LdapWrapper.c ./db/LdapWrapper/LdapWrapper.h
	gcc -c ./db/LdapWrapper/LdapWrapper.c
	
ClientConnection.o: ./connection/ClientConnection.c ./connection/ClientConnection.h
	gcc -c ./connection/ClientConnection.c
	
HttpConnectionDriver.o: ./connection/HttpConnectionDriver.c ./connection/HttpConnectionDriver.h
	gcc -c ./connection/HttpConnectionDriver.c
	
MemCachedService.o: ./db/MemCachedService.c ./db/MemCachedService.h
	gcc -c ./db/MemCachedService.c
	
.PHONY : clean

clean:
	rm $(NOTICE) $(CONNECTION) $(DB) $(LDAP) $(SERVER) $(UTILS)