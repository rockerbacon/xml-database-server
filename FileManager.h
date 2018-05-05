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

namespace tebd {

	class FileManager {
		private:
			std::list<std::experimental::filesystem::directory_entry> que_files;
			
			std::atomic_bool b_fileCheck;
			std::thread *thd_fileCheck;
			
			std::string s_folderPath;
			
			void _p_fileCheck (unsigned short updateInterval);
			
		public:
			FileManager (std::string &folderPath);
			~FileManager (void);
			
			//intervalo de atualizacao em millisegundos
			void startFileCheck (unsigned short updateInterval);
			void stopFileCheck (void);
			
			inline std::thread* getFileCheckThread (void) {
				return this->thd_fileCheck;
			}
	};

};
