#ifndef FUNCTION_DEF_H
#define FUNCTION_DEF_H

#include "AstNode.h"
#include "../SymbolTable.h"

namespace tkom {
	namespace ast {

		class FunctionDef : public Node
		{
		public:
			Symbol::DataType returnType;
			std::unique_ptr<CallDef> requiredArguments;
			std::unique_ptr<Body> functionBody;
		};

	}
}

#endif // !FUNCTION_DEF_H