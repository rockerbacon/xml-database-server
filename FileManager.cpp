#include "FileManager.h"
#include <chrono>
#include <ctime>
#include <iostream>

using namespace tebd;
using namespace std;

FileManager::FileManager (const std::string &s_folderPath) {
	this->b_fileCheck = false;
	this->s_folderPath = s_folderPath;
	this->thd_fileCheck = NULL;
}

FileManager::~FileManager (void) {
	this->stopFileCheck();
	if (this->thd_fileCheck != NULL) delete (this->thd_fileCheck);
}

void FileManager::_p_fileCheck (unsigned short updateInterval) {
	namespace fs = std::experimental::filesystem;
	std::time_t t_lastUpdate = 0;
	
	while (this->b_fileCheck) {
		
		bool updated = false;
		for (auto &file : fs::directory_iterator(fs::current_path()/this->s_folderPath)) {
			//TODO armazenar somente novos arquivos em this->fileQueue
			auto ftime = fs::last_write_time(file); 
			std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
			
			if (cftime > t_lastUpdate) {
				this->que_files.push_back(file);
				updated = true;
			}
			
			//TODO filtrar arquivos invalidos
		}
		if (updated) t_lastUpdate = std::time(NULL);
		std::this_thread::sleep_for(std::chrono::duration<unsigned short, std::milli>(updateInterval));
	}

}

void FileManager::startFileCheck (unsigned short updateInterval) {
	if (!this->b_fileCheck) {
		this->b_fileCheck = true;
		if (this->thd_fileCheck != NULL) delete (this->thd_fileCheck);
		this->thd_fileCheck = new thread(&FileManager::_p_fileCheck, this, updateInterval);
	}
}

void FileManager::stopFileCheck (void) {
	if (this->b_fileCheck) {
		this->b_fileCheck = false;
	}
}
