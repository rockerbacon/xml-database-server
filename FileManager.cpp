#include "FileManager.h"
#include <experimental/filesystem>

using namespace tebd;
using namespace std;

FileManager::FileManager (std::string &s_folderPath) {
	this->b_fileCheck = false;
	this->s_folderPath = s_folderPath;
}

FileManager::~FileManager (void) {
	this->stopFileCheck();
}

void FileManager::_p_fileCheck (void) {
	namespace fs = std::experimental::filesystem;
	while (this->b_fileCheck) {
		
		for (auto file : fs::directory_iterator(this->s_folderPath)) {
			//TODO armazenar somente novos arquivos em this->fileQueue
		}
		
	}

}

void FileManager::startFileCheck (void) {
	if (!this->b_fileCheck) {
		this->b_fileCheck = true;
		this->thd_fileCheck = new thread(&FileManager::_p_fileCheck, this);
	}
}

void FileManager::stopFileCheck (void) {
	if (this->b_fileCheck) {
		this->b_fileCheck = false;
		delete(this->thd_fileCheck);
	}
}
