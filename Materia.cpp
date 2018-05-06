#include "Materia.h"

using namespace std;
using namespace tebd;
using namespace xercesc;

string Materia::tag = "materia";

void Materia::insertIntoDb (void) {

}

void Materia::buildFromXml (DOMNode *node) {
	DOMElement *element = dynamic_cast<DOMElement*>(node);
	XMLCh* tag_codigo = XMLString::transcode("codigo");
	const XMLCh* xch_codigo = element->getAttribute(tag_codigo);
	XMLCh* tag_nome = XMLString::transcode("nome");
	const XMLCh* xch_nome = element->getAttribute(tag_nome);
	XMLCh* tag_creditos = XMLString::transcode("creditos");
	const XMLCh* xch_creditos = element->getAttribute(tag_creditos);
	XMLCh* tag_conceito = XMLString::transcode("conceito");
	const XMLCh* xch_conceito = element->getAttribute(tag_conceito);
	XMLCh* tag_situacao = XMLString::transcode("situacao");
	const XMLCh* xch_situacao = element->getAttribute(tag_situacao);
	
	this->codigo = XMLString::transcode(xch_codigo);
	this->nome = XMLString::transcode(xch_nome);
	this->creditos = stoi(XMLString::transcode(xch_creditos));
	this->conceito = stod(XMLString::transcode(xch_conceito));
	this->situacao = XMLString::transcode(xch_situacao);
	
	XMLString::release(&tag_codigo);
	XMLString::release(&tag_nome);
	XMLString::release(&tag_creditos);
	XMLString::release(&tag_conceito);
	XMLString::release(&tag_situacao);
}
