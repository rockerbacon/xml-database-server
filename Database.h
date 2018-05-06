#pragma once

#include <sqlite3.h>
#include <string>

namespace tebd {

	class Database {
	
		private:
			sqlite3 *db;
	
		public:
			//operador & antes da variavel indica que argumento deve ser passado por referencia
			Database();
			virtual ~Database();
		
			int execute_insert(std::string tabela, std::string valores);
	
	};

};
