#pragma once

#include "DbItem.h"
#include <string>

namespace tebd {

	class Aluno : public DbItem {
	
		private:
			std::string cpf;
			std::string nome;
			std::string universidade;
			std::string curso;
	
		public:
			//operador & antes da variavel indica que argumento deve ser passado por referencia
			Aluno(std::string nomeaux, std::string cpfaux, std::string universidadeaux, std::string cursoaux): cpf(cpfaux), nome(nomeaux), universidade(universidadeaux),curso(cursoaux){};
		
			virtual void insertIntoDb (void);
	
	};

};
