#include "FileManager.h"
#include <chrono>
#include <ctime>

using namespace tebd;
using namespace std;

FileManager::FileManager (std::string &s_folderPath) {
	this->b_fileCheck = false;
	this->s_folderPath = s_folderPath;
}

FileManager::~FileManager (void) {
	this->stopFileCheck();
}

void FileManager::_p_fileCheck (unsigned short updateInterval) {
	namespace fs = std::experimental::filesystem;
	std::time_t t_lastUpdate = 0;
	while (this->b_fileCheck) {
		
		for (auto &file : fs::directory_iterator(this->s_folderPath)) {
			//TODO armazenar somente novos arquivos em this->fileQueue
			auto ftime = fs::last_write_time(file); 
			std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
			
			if (cftime > t_lastUpdate) {
				this->que_files.push_back(file);
			}
			
			//TODO filtrar arquivos invalidos
		}
		t_lastUpdate = std::time(NULL);
		std::this_thread::sleep_for(std::chrono::duration<unsigned short, std::milli>(updateInterval));
	}

}

void FileManager::startFileCheck (unsigned short updateInterval) {
	if (!this->b_fileCheck) {
		this->b_fileCheck = true;
		this->thd_fileCheck = new thread(&FileManager::_p_fileCheck, this, updateInterval);
	}
}

void FileManager::stopFileCheck (void) {
	if (this->b_fileCheck) {
		this->b_fileCheck = false;
		delete(this->thd_fileCheck);
	}
}
