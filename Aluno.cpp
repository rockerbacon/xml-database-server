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
	const XMLCh* xch_cpf = element->getAttribute(tag_cpf);	//recupera valor da tag especificada. Valor eh somente referencia para valor dentro do parser, portanto deve ser constante e nao pode sofrer release apos uso
	XMLCh* tag_nome = XMLString::transcode("nome");
	const XMLCh* xch_nome = element->getAttribute(tag_nome);
	XMLCh* tag_universidade = XMLString::transcode("universidade");
	const XMLCh* xch_universidade = element->getAttribute(tag_universidade);
	XMLCh* tag_curso = XMLString::transcode("curso");
	const XMLCh* xch_curso = element->getAttribute(tag_curso);
	
	//armazenar dados no objeto
	this->cpf = XMLString::transcode(xch_cpf);
	this->nome = XMLString::transcode(xch_nome);
	this->universidade = XMLString::transcode(xch_universidade);
	this->curso = XMLString::transcode(xch_curso);
	
	//strings xml precisam passar por um release apos uso
	XMLString::release(&tag_cpf);
	XMLString::release(&tag_nome);
	XMLString::release(&tag_universidade);
	XMLString::release(&tag_curso);
}
