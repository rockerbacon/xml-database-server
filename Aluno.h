#pragma once

#include "DbItem.h"
#include "XMLItem.h"
#include <string>

namespace tebd {

	class Aluno : public DbItem, public XMLItem {
	
		private:
			static std::string tag;
			std::string cpf;
			std::string nome;
			std::string universidade;
			std::string curso;
	
		public:
			//operador & antes da variavel indica que argumento deve ser passado por referencia
			Aluno (void) = default;
			Aluno(std::string nomeaux, std::string cpfaux, std::string universidadeaux, std::string cursoaux): cpf(cpfaux), nome(nomeaux), universidade(universidadeaux),curso(cursoaux){};
		
			virtual void insertIntoDb (void);
			
			inline virtual std::string getTag (void) { return Aluno::tag; }
			virtual void buildFromXml (xercesc::DOMNode *node);
			
			inline std::string getCpf (void) { return this->cpf; }
	
	};

};
