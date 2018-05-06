#include "FileManager.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax/HandlerBase.hpp>

using namespace tebd;
using namespace std;
using namespace xercesc;

unsigned int FileManager::_instanceCount = 0;

FileManager::FileManager (const string &s_folderPath, const string &xsd) throw (XMLException) {
	this->b_fileCheck = false;
	this->s_folderPath = realpath(s_folderPath.c_str(), NULL);
	this->thd_fileCheck = NULL;
	this->s_xsdPath = realpath(xsd.c_str(), NULL);
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
			
			if (this->_validate(this->s_xsdPath, file.path().string())) {
				if (cftime > t_lastUpdate) {
					this->que_files.push_back(file);
					updated = true;
				}
			} else {
				//TODO log erro de arquivo invalido
			}
		}
		if (updated) t_lastUpdate = std::time(NULL);
		std::this_thread::sleep_for(std::chrono::duration<unsigned short, std::milli>(updateInterval));
	}

}

bool FileManager::_validate (const string &xsdPath, const string &xmlPath) {
	XercesDOMParser domParser;     
	if (domParser.loadGrammar(xsdPath.c_str(), Grammar::SchemaGrammarType) == NULL) {
		throw exception();	//TODO improve exception handling
	}
	ErrorHandler *err = (ErrorHandler*) new HandlerBase();

	domParser.setErrorHandler(err);
	domParser.setValidationScheme(XercesDOMParser::Val_Always);
	domParser.setDoNamespaces(true);
	domParser.setDoSchema(true);
	domParser.setValidationSchemaFullChecking(true);
	domParser.setExternalNoNamespaceSchemaLocation(xsdPath.c_str());

	try {
		domParser.parse(xmlPath.c_str());
	} catch (XMLException &e) {
		char *message = XMLString::transcode(e.getMessage());
		//TODO log error message
		XMLString::release(&message);
	}
	delete err;
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
