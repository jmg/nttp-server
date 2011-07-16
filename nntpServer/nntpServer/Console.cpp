#ifndef Console_CPP
#define Console_CPP

#define DllExport __declspec(dllexport)
#include <iostream>
#include <string>

using namespace std;

class Console 
{
	public:
		Console() {
		}

		static string Read() {
			string s;
			getline(cin, s);
			return s;
		}

		static void WriteLine(string s) {
			cout << s << endl;
		}

		static void WriteLine(long s) {
			cout << s << endl;
		}

		static void Write(string s) {
			cout << s;
		}
};
#endif