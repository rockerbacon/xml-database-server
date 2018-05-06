#pragma once

#include "DbItem.h"
#include "XMLItem.h"
#include <string>

namespace tebd {
	class Materia : public DbItem, public XMLItem {
		private:
			static std::string tag;
			
			std::string codigo;
			int creditos;
			double conceito;
			std::string situacao;
			
		public:
			virtual void insertIntoDb (void);
			
			inline virtual std::string getTag (void) { return Materia::tag; }
			virtual void buildFromXml (DOMNode *node);
		
	};
};
