#include "FileManager.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include "Log.h"

using namespace tebd;
using namespace std;
using namespace xercesc;

unsigned int FileManager::_instanceCount = 0;

FileManager::FileManager (const string &s_folderPath, const string &xsd) throw (XMLException) {
	this->b_fileCheck = false;
	this->b_fileParse = false;
	this->s_folderPath = realpath(s_folderPath.c_str(), NULL);
	this->thd_fileCheck = NULL;
	this->s_xsdPath = realpath(xsd.c_str(), NULL);
	if (FileManager::_instanceCount == 0) {
		XMLPlatformUtils::Initialize();
	}
	FileManager::_instanceCount++;
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
		for (auto &file : fs::directory_iterator(this->s_folderPath)) {
			auto ftime = fs::last_write_time(file); 
			std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
			
			if (cftime > t_lastUpdate) {
				if (this->_validate(this->s_xsdPath, file.path().string())) {
						this->que_files.load().push_back(file);
						updated = true;
				} else {
					log("Arquivo "+file.path().string()+" possui formato invalido");
				}
			}
		}
		if (updated) t_lastUpdate = std::time(NULL);
		std::this_thread::sleep_for(std::chrono::duration<unsigned short, std::milli>(updateInterval));
	}

}

void FileManager::_p_fileParse (void) {
	XercesDOMParser parser;
	XMLCh *tag_method = XMLString::transcode("methodName");
	XMLCh *tag_params = XMLString::transcode("params");
	XMLCh *tag_param = XMLString::transcode("param");
	XMLCh *tag_boletim = XMLString::transcode("boletim");
	XMLCh *tag_historico = XMLString::transcode("historico");
	
	parser.setValidationScheme(XercesDOMParser::Val_Never);
	parser.setDoNamespaces(false);
	parser.setDoSchema(false);
	parser.setLoadExternalDTD(false);
	
	while (this->b_fileParse) {
		if (this->que_files.load().size() > 0) {
			string s_file = this->que_files.load().front().string();
			this->que_files.load().pop_front();
			
			try {
			
				parser.parse(s_file);
				DOMDocument *doc = parser.getDocument();
				DOMElement *node = doc->getDocumentElement();
				if (node == NULL) {
					log("Empty document "+s_file);
					continue;
				}
				
				const XMLCh *method = node->getAttribute(tag_method);
				
				DOMNodeList *l_children = node->getChildNodes();
				const XMLSize_t i_childrenLength = l_children->getLength();
				
				//buscar chamadas de metodos
				for (XMLSize_t i = 0; i < i_childrenLength; ++i) {
					DOMNode *child = n->item(i);
					DOMElement *e = dynamic_cast<DOMElement*>(child);
					if (XMLString::equals(e->getTagName(), tag_method)) {
						const XMLCh *method = e->getAttribute(tag_method);
					} else if (XMLString::equals(e->getTagName(), tag_param)) {
					
					}
					
				}
				
			} catch (XMLException &e) {
				
			}
			
		}
	}
	
	XMLString::release(&tag_method);
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
		log(message);
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

void FileManager::startFileParse (void) {
	if (!this->b_fileParse) {
		this->b_fileParse = true;
		if (this->thd_fileParse != NULL) delete (this->thd_fileParse);
		this->thd_fileParse = new thread(&FileManager::_p_fileParse, this);
	}
}

void FileManager::stopFileParse (void) {
	if (this->b_fileParse) {
		this->b_fileParse = false;
	}
}
