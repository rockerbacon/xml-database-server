#include "Materia.h"

using namespace std;
using namespace tebd;
using namespace xercesc;

string Materia::tag = "materia";

void Materia::insertIntoDb (void) {

}

void Materia::buildFromXml (DOMNode *node) {
	XMLCh* tag_codigo = XMLString::transcode("codigo");
	XMLCh* tag_nome = XMLString::transcode("nome");
	XMLCh* tag_creditos = XMLString::transcode("creditos");
	XMLCh* tag_conceito = XMLString::transcode("conceito");
	XMLCh* tag_situacao = XMLString::transcode("situacao");
	
	DOMNodeList *l_children = node->getChildNodes();
	for (XMLSize_t i = 0; i < l_children->getLength(); ++i) {
		DOMNode *n = l_children->item(i);
		const XMLCh *tag = n->getNodeName();
		if (XMLString::equals(tag, tag_codigo)) {
			this->codigo = XMLString::transcode(n->getNodeValue());
		} else if (XMLString::equals(tag, tag_nome)) {
			this->nome = XMLString::transcode(n->getNodeValue());
		} else if (XMLString::equals(tag, tag_creditos)) {
			this->creditos = stoi(XMLString::transcode(n->getNodeValue()));
		} else if (XMLString::equals(tag, tag_conceito)) {
			this->conceito = stod(XMLString::transcode(n->getNodeValue()));
		} else if (XMLString::equals(tag, tag_situacao)) {
			this->situacao = XMLString::transcode(n->getNodeValue());
		}
	}
	
	XMLString::release(&tag_codigo);
	XMLString::release(&tag_nome);
	XMLString::release(&tag_creditos);
	XMLString::release(&tag_conceito);
	XMLString::release(&tag_situacao);
}
