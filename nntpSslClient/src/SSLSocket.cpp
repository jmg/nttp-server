#include <openssl/ssl.h>
#include <openssl/err.h>
#include "Socket.cpp"

#define END '.'
#define SOCKETERROR -1

class SSLSocket : public Socket
{
	private:
		SSL* sslHandle;
		SSL_CTX *sslCxt;
		static const unsigned int READSIZE = 8;

	public:

		SSLSocket(string host, int port)
		: Socket(host, port)
		{
			sslHandle = NULL;
			sslCxt = NULL;

			InitializeCtx();
            SSLConnect();
		}

        void SSLConnect()
        {
            sslHandle = SSL_new(sslCxt);
            SSL_set_fd(sslHandle, sockfd);
            if (SSL_connect(sslHandle) == -1)
                ERR_print_errors_fp(stderr);
            else
                printf("Conectado! usando encriptado: %s\n", SSL_get_cipher(sslHandle));
        }

		void InitializeCtx()
		{
		    SSL_library_init();
            OpenSSL_add_all_algorithms();
            SSL_load_error_strings();
		    sslCxt = SSL_CTX_new(SSLv23_client_method());
            if (sslCxt == NULL )
            {
                ERR_print_errors_fp(stderr);
                abort();
            }
		}

		int isFinishOfMessage(char* buffer)
		{
			return (buffer[8] == END && buffer[9] == '\n' || buffer[8] == '\r' && buffer[9] == '\n');
		}

		string read()
		{
			int i;
			int received;
			char* buffer = (char*) calloc(BUFFER, sizeof(char));
			char* rc = (char*) calloc(BUFFER, sizeof(char));

			for (i=1;;i++)
			{
				received = SSL_read(sslHandle, buffer, BUFFER);
				if (received > 0)
				{
					rc = (char*) (realloc(rc, i * (BUFFER+1) * sizeof(char)));
					StrMng::concatString(rc, buffer);
				}
				else if(received < 0)
				{
					exit(1);
				}
				if (received < BUFFER || isFinishOfMessage(buffer))
					break;
				memset(buffer, '\0', BUFFER);
			}

			free(buffer);
			return StrMng::getString(rc);
		}

		int write (string text)
		{
			if (SSL_write(sslHandle, StrMng::ToCharArray(text), text.length()) < 0)
				return SOCKETERROR;
			else
				return 0;
		}

		void Close()
		{
			SSL_free(sslHandle);
			close(sockfd);
			SSL_CTX_free(sslCxt);
		}
};

