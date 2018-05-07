#include "FileManager.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include "Log.h"
#include "Aluno.h"
#include "Periodo.h"
#include <stdio.h>

using namespace tebd;
using namespace std;
using namespace xercesc;

unsigned int FileManager::_instanceCount = 0;

FileManager::FileManager (const string &s_folderPath, const string &xsd) throw (XMLException) {
	this->b_fileCheck = false;
	this->b_fileParse = false;
	this->s_folderPath = realpath(s_folderPath.c_str(), NULL);
	this->thd_fileCheck = NULL;
	this->thd_fileParse = NULL;
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
						//TODO adicionar lock
						this->que_files.push_back(file);
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
	XMLCh *tag_historico = XMLString::transcode("historico");
	string request_method;
	
	parser.setLoadExternalDTD(false);
	
	while (this->b_fileParse) {
		if (this->que_files.size() > 0) {
			//TODO adicionar lock
			string s_file = this->que_files.front().path().string();
			this->que_files.pop_front();
			
			try {
			
				parser.parse(s_file.c_str());
				DOMDocument *doc = parser.getDocument();
				DOMElement *node = doc->getDocumentElement();
				if (node == NULL) {
					log("Empty document "+s_file);
					continue;
				}
				
				DOMNodeList *l_l1 = node->getChildNodes();
				for (XMLSize_t i = 0; i < l_l1->getLength(); ++i) {
					//dentro de methodCall
					DOMNode *n = l_l1->item(i);
					const XMLCh* tag = n->getNodeName();
					if (XMLString::equals(tag, tag_method)) {
						const XMLCh *tmp = n->getTextContent();
						request_method = XMLString::transcode(tmp);
					} else if (XMLString::equals(tag, tag_params)) {
						DOMNodeList *l_l2 = n->getChildNodes();
						
						for (XMLSize_t j = 0; j < l_l2->getLength(); ++j) {
							//dentro de params
							DOMNode *n2 = l_l2->item(j);
							DOMNodeList *l_l3 = n2->getChildNodes();
							for (XMLSize_t k = 0; k < l_l3->getLength(); ++k) {
								//dentro de param
								DOMNode *n3 = l_l3->item(k);
								DOMNodeList *l_l4 = n3->getChildNodes();
								Aluno a_add;
								XMLCh *tag_aluno = XMLString::transcode(a_add.getTag().c_str());
								
								for (XMLSize_t l = 0; l < l_l4->getLength(); ++l) {
									//dentro de boletim
									DOMNode *n4 = l_l4->item(l);
									const XMLCh *tag2 = n4->getNodeName();
									if (XMLString::equals(tag2, tag_aluno)) {
										a_add.buildFromXml(n4);
										a_add.insertIntoDb();
									} else if (XMLString::equals(tag2, tag_historico)) {
										DOMNodeList *l_l5 = n4->getChildNodes();
										Periodo p_add(a_add.getCpf());
										for (XMLSize_t m = 0; m < l_l5->getLength(); ++m) {
											//dentro de historico
											DOMNode *n5 = l_l5->item(m);
											p_add.buildFromXml(n5);
											p_add.insertIntoDb();
										}
									}
								}
								
								XMLString::release(&tag_aluno);	
							}
						}
					}
				}
				
			} catch (XMLException &e) {
				string message = XMLString::transcode(e.getMessage());
				log(message);
			}
			
			//remove(s_file.c_str());	//apagar arquivos ja processados
			
		}
	}
	
	XMLString::release(&tag_method);
	XMLString::release(&tag_params);
	XMLString::release(&tag_historico);
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
