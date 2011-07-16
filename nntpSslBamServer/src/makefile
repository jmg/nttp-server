DB = DatabaseService.o
LDAP = LdapService.o LdapWrapper.o
UTILS = Config.o Logs.o Stack.o StrMng.o
NOTICE = Notice.o NoticeGroups.o
SOCKET = SSLSocket.o
NNTPPARSER = NNTPParser.o

LIBS = -L/usr/local/ssl/lib -lssl -lcrypto -ldl -lldap
INCLUDE= -I/usr/local/ssl/include/

NntpSSLBamServer: $(NNTPPARSER) $(NOTICE) $(SOCKET) $(LDAP) $(DB) $(UTILS)
	gcc nntpSSlBamServer.c $(NNTPPARSER) $(NOTICE) $(SOCKET) $(LDAP) $(DB) $(UTILS) $(LIBS) $(INCLUDE) -ansi -o	nntpSSLBamServer  

DatabaseService.o: ./OpenDs/DatabaseService.c	./OpenDs/DatabaseService.h
	gcc -c ./OpenDs/DatabaseService.c

LdapService.o:	./OpenDs/LdapService.c	./OpenDs/LdapService.h
	gcc -c ./OpenDs/LdapService.c

LdapWrapper.o: ./OpenDs/wrapper/LdapWrapper.c ./OpenDs/wrapper/LdapWrapper.h
	gcc -c ./OpenDs/wrapper/LdapWrapper.c

Config.o: ./Utils/Config.c ./Utils/Config.h
	gcc -c ./Utils/Config.c	

Logs.o: ./Utils/Logs.c ./Utils/Logs.h
	gcc -c ./Utils/Logs.c

Stack.o: ./Utils/Stack.c ./Utils/Stack.h
	gcc -c ./Utils/Stack.c

StrMng.o: ./Utils/StrMng.c ./Utils/StrMng.h
	gcc -c ./Utils/StrMng.c

Notice.o:	./Notice.c	./Notice.h
	gcc -c ./Notice.c

NoticeGroups.o:		./NoticeGroups.c	./NoticeGroups.h
	gcc -c ./NoticeGroups.c

SSLSocket.o:		./SSLSocket.c	./SSLSocket.h
	gcc -c ./SSLSocket.c

NNTPParser.o:		./NNTPParser.c	./NNTPParser.h
	gcc -c ./NNTPParser.c

	
.PHONY : clean

clean:
	rm $(DB) $(LDAP) $(UTILS) $(NOTICE) $(SOCKET) $(NNTPPARSER)