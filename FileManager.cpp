#include "FileManager.h"
#include <experimental/filesystem>
#include <chrono>

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
	while (this->b_fileCheck) {
		
		for (auto file : fs::directory_iterator(this->s_folderPath)) {
			//TODO armazenar somente novos arquivos em this->fileQueue
		}
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
