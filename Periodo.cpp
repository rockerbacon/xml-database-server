#include "Periodo.h"

using namespace std;
using namespace tebd;
using namespace xercesc;

string Periodo::tag = "periodo";

Periodo::Periodo (const string &cpf_aluno) {
	this->cpf_aluno = cpf_aluno;
}

void Periodo::insertIntoDb (void) {

}

void Periodo::buildFromXml (DOMNode *node) {
	XMLCh *tag_ano = XMLString::transcode("ano");
	XMLCh *tag_semestre = XMLString::transcode("semestre");
	Materia m_add;
	XMLCh *tag_m = XMLString::transcode(m_add.getTag().c_str());
	
	DOMNodeList *l_children = element->getChildNodes;
	//explorar node para recuperar materias
	for (XMLSize_t i = 0; i < l_children->getLength(); ++i) {
		DOMNode *n = l_children->item(i);
		XMLCh *tag = n->getName();
		if (XMLString::Equals(tag, tag_ano)) {
			this->ano = XMLString::transcode(n->getNodeValue());
		} else if (XMLString::Equals(tag, tag_semestre)) {
			this->semestre = XMLString::transcode(n->getNodeValue());
		} else if (XMLString::Equals(tag, tag_m)) {
			
			m_add.buildFromXml(n);
			m_add.insertIntoDb();
			this->l_materia.push_back(m_add.getCodigo());
			
		}
	}
	
	XMLString::release(&tag_ano);
	XMLString::release(&tag_semestre);
	XMLString::release(&tag_m);
}
