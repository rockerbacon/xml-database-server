#pragma once

#include <string>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMElement.hpp>

namespace tebd {
	class XMLItem {
		public:
			virtual std::string getTag (void) = 0;
			virtual void buildFromXml (xercesc::DOMNode *node) = 0;
	};
};
