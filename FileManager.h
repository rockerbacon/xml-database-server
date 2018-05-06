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
			static unsigned int _instanceCount;
		
			std::atomic<std::list<std::experimental::filesystem::directory_entry>> que_files;
			
			std::atomic_bool b_fileCheck;
			std::thread *thd_fileCheck;
			
			std::atomic_bool b_fileParse;
			std::thread *thd_fileParse;
			
			std::string s_folderPath;
			std::string s_xsdPath;
			
			void _p_fileCheck (unsigned short updateInterval);
			void _p_fileParse (void);
			
			//retorna true quando xml eh valido e false caso contrario
			bool _validate (const std::string &xsdPath, const std::string &xmlPath);
			
		public:
			FileManager (const std::string &folderPath, const std::string &xsd) throw (xercesc::XMLException);
			~FileManager (void);
			
			//intervalo de atualizacao em millisegundos
			void startFileCheck (unsigned short updateInterval);
			void stopFileCheck (void);
			
			void startFileParse (void);
			void stopFileParse (void);
			
			inline std::thread* getFileCheckThread (void) {
				return this->thd_fileCheck;
			}
			
			inline std::thread* getFileParseThread (void) {
				return this->thd_fileParse;
			}
			
			inline int getFileCount (void) {
				return this->que_files.size();
			}
	};

};
