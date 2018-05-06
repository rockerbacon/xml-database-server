#include "Log.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>

using namespace std;

std::string tebd::s_logPath = "error.log";

void tebd::log (const string &message) {
	fstream f_out(s_logPath, fstream::app|fstream::out);
	time_t t_stamp = time(NULL);
	tm *t_current = localtime(&t_stamp);
	
	f_out << put_time(t_current, "%d/%m/%y %H:%M: ") << message << endl;
	f_out.close();
}
