#ifndef CALL_DEF_H
#define CALL_DEF_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class CallDef : public Node
		{
		public:
			std::vector<std::pair<Symbol::DataType, std::string>> argumentsList;
		};

	}
}

#endif // !CALL_DEF_H