#include "FileManager.h"
#include <chrono>
#include <ctime>
#include <iostream>

using namespace tebd;
using namespace std;
using namespace xercesc;

unsigned int FileManager::_instanceCount = 0;

FileManager::FileManager (const string &s_folderPath, const string &xsd) throw (XMLException) {
	this->b_fileCheck = false;
	this->s_folderPath = s_folderPath;
	this->thd_fileCheck = NULL;
}

FileManager::~FileManager (void) {
	this->stopFileCheck();
	if (this->thd_fileCheck != NULL) delete (this->thd_fileCheck);
	if (FileManager::_instanceCount == 1) {
		XMLPlatformUtils::Terminate();
	}
	FileManager::_instanceCount--;
}

void FileManager::_p_fileCheck (unsigned short updateInterval) {
	namespace fs = std::experimental::filesystem;
	std::time_t t_lastUpdate = 0;
	
	while (this->b_fileCheck) {
		
		bool updated = false;
		for (auto &file : fs::directory_iterator(fs::current_path()/this->s_folderPath)) {
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

bool FileManager::_validate (const string &xsdPath, const string &xmlPath) {
	XercesDOMParser domParser;     
	if (domParser.loadGrammar(xsdPath, Grammar::SchemaGrammarType) == NULL) {
		throw Except("Unable to load xsd");
	}

	ParserErrorHandler parserErrorHandler;

	domParser.setErrorHandler(&parserErrorHandler);
	domParser.setValidationScheme(XercesDOMParser::Val_Always);
	domParser.setDoNamespaces(true);
	domParser.setDoSchema(true);
	domParser.setValidationSchemaFullChecking(true);
	domParser.setExternalNoNamespaceSchemaLocation(xsd);

	domParser.parse(xmlPath);
	if(domParser.getErrorCount() != 0) {     
		return false;
	}
	return true;
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
