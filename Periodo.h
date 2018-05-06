#pragma once

#include "DbItem.h"
#include "XMLItem.h"
#include <string>
#include <list>

namespace tebd {
	class Periodo : public DbItem, public XMLItem {
		private:
			static std::string tag;
			
			std::string cpf_aluno;
			std::string ano;
			std::string semestre;
			std::list<std::string> l_materia;	//lista com codigo de materias
			
		public:
			virtual void insertIntoDb (void);
			
			inline virtual std::string getTag (void) { return Periodo::tag; }
			virtual void buildFromXml (DOMNode *node);
		
	};
};
