#include "FileManager.h"
#include <iostream>

using namespace std;
using namespace tebd;

int main (void) {

	FileManager m("requests","methodCall.xsd");
	int a;
	
	m.startFileCheck(500);
	m.startFileParse();
	
	do {
		cout << "Processando arquivos, digite 0 para sair" << endl;
		cin >> a;
	} while (a != 0);
	
	m.stopFileCheck();
	m.stopFileParse();
	m.getFileCheckThread()->join();
	m.getFileParseThread()->join();

	return 0;
}
