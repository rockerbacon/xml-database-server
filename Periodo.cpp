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
	DOMElement *element = dynamic_cast<DOMElement*>(node);
	XMLCh *tag_ano = XMLString::transcode("ano");
	const XMLCh *xch_ano = element->getAttribute(tag_ano);
	XMLCh *tag_semestre = XMLString::transcode("semestre");
	const XMLCh *xch_semestre = element->getAttribute(tag_semestre);
	Materia m_add;
	XMLCh *tag_m = XMLString::transcode(m_add.getTag().c_str());
	
	DOMNodeList *n_children;
	XMLSize_t i_childrenCount;
	
	this->ano = XMLString::transcode(xch_ano);
	this->semestre = XMLString::transcode(xch_semestre);
	
	//explorar node para recuperar materias
	n_children = element->getChildNodes();
	i_childrenCount = n_children->getLength();
	for (XMLSize_t i = 0; i < i_childrenCount; ++i) {
		DOMNode *n = n_children->item(i);
		if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
			DOMElement *e = dynamic_cast<DOMElement*>(n);
			if (XMLString::equals(e->getTagName(), tag_m)) {
			
				//verificado tag correta chamar construcao de materia e adicionar ao banco
				m_add.buildFromXml(n);
				m_add.insertIntoDb();
				this->l_materia.push_back(m_add.getCodigo());
			
			}
		}
	}
	
	XMLString::release(&tag_ano);
	XMLString::release(&tag_semestre);
	XMLString::release(&tag_m);
}
