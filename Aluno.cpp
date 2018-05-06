#include "Aluno.h"
#include "Database.h"

using namespace tebd;
using namespace std;
using namespace xercesc;

string Aluno::tag = "aluno";

//TODO implementar metodo de insercao de aluno no banco
void tebd::Aluno::insertIntoDb (void) {
    Database db;
    std::string valores = "'"+nome+"','"+cpf+"','"+universidade+"','"+curso+"'";
    db.execute_insert("ALUNO",valores);
}

void Aluno::buildFromXml (DOMNode *node) {
	DOMElement *element = dynamic_cast<DOMElement*>(node);
	//recuperar dados do xml
	XMLCh* tag_cpf = XMLString::transcode("cpf");	//strings do xml usam o tipo XMLCh
	XMLCh* tag_nome = XMLString::transcode("nome");
	XMLCh* tag_universidade = XMLString::transcode("universidade");
	XMLCh* tag_curso = XMLString::transcode("curso");
	
	//armazenar dados no objeto
	DOMNodeList *l_children = node->getChildNodes();
	for (XMLSize_t i = 0; i < l_children->getLength(); ++i) {
		DOMNode *n = l_children->item(i);
		XMLCh *tag = n->getName();
		if (XMLString::Equals(tag, tag_cpf)) {
			this->cpf = XMLString::transcode(n->getNodeValue());
		} else if (XMLString::Equals(tag, tag_nome)) {
			this->nome = XMLString::transcode(n->getNodeValue());
		} else if (XMLString::Equals(tag, tag_universidade)) {
			this->universidade = XMLString::transcode(n->getNodeValue());
		} else if (XMLString::Equals(tag, tag_curso)) {
			this->curso = XMLString::transcode(n->getNodeValue());
		}
	}
	
	//strings xml precisam passar por um release apos uso
	XMLString::release(&tag_cpf);
	XMLString::release(&tag_nome);
	XMLString::release(&tag_universidade);
	XMLString::release(&tag_curso);
}
