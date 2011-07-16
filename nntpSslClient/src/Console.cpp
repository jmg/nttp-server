#ifndef CONSOLE_CPP
#define CONSOLE_CPP
#include <iostream>

using namespace std;

//Clase estatica que maneja el I/O de la consola
class Console {

public:
	Console() {
	}
	//Lee de STDIN
	static string Read() {
		string s;
		getline(cin, s);
		return s;
	}
	//Escribe en STDOUT con un "\n"
	static void WriteLine(string s) {
		cout << s << endl;
	}
	//Escribe en STDOUT
	static void Write(string s) {
		cout << s;
	}
};

#endif
