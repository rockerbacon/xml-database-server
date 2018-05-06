/*
 * Um file manager eh um gerenciador que verifica o surgimento de novos arquivos em uma pasta e informa que esses arquivos estao prontos para serem processados
 *
 *
 */

#pragma once

#include <string>
#include <list>
#include <atomic>
#include <thread>
#include <experimental/filesystem>
#include <xercesc/util/PlatformUtils.hpp>

namespace tebd {

	class FileManager {
		private:
			unsigned int _instanceCount;
		
			std::list<std::experimental::filesystem::directory_entry> que_files;
			
			std::atomic_bool b_fileCheck;
			std::thread *thd_fileCheck;
			
			std::string s_folderPath;
			
			xercesc::XercesDOMParser parser;
			
			void _p_fileCheck (unsigned short updateInterval);
			
			//retorna true quando xml eh valido e false caso contrario
			bool _validate (const std::string &xsdPath, const std::string &xmlPath);
			
		public:
			FileManager (const std::string &folderPath, const std::string &xsd) throw (xercesc::XMLException);
			~FileManager (void);
			
			//intervalo de atualizacao em millisegundos
			void startFileCheck (unsigned short updateInterval);
			void stopFileCheck (void);
			
			inline std::thread* getFileCheckThread (void) {
				return this->thd_fileCheck;
			}
			inline int getFileCount (void) {
				return this->que_files.size();
			}
	};

};
