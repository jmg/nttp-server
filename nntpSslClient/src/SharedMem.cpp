#ifndef SHAREDMEM_CPP
#define SHAREDMEM_CPP
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <cstring>
#include "Random.cpp"
#include "StrMng.cpp"
#include "Console.cpp"

using namespace std;

class SharedMem
{
	public:
		//Crea un area de memoria aleatoria para compartirla
		SharedMem()
		{
			key = Random::Gen();
			shmid = shmget(key, SIZE, IPC_CREAT | 0666);
			if (shmid==-1) {
				Console::WriteLine("Error obteniendo id para memoria compartida");
				exit(1);
			}

			shm = (char*) shmat(shmid, NULL, 0);
			
			if (shm==NULL) {
				Console::WriteLine("Error obteniendo memoria compartida");
				exit(1);
			}
		}
		void Write(string str)
		{
			strcpy(shm, StrMng::ToCharArray(str));
		}
		string Read()
		{
			shmid = shmget(key, SIZE, 0666);
			shm = (char*)shmat(shmid, NULL, 0);
			return StrMng::getString(shm, false);
		}
		~SharedMem()
		{
			if (shmctl(shmid, IPC_RMID, NULL) == -1)
				perror("Failed to  remove memory segment");
		}

	private:
		static const int SIZE = 30000;
		int shmid;
		key_t key;
		char *shm;
};
#endif
