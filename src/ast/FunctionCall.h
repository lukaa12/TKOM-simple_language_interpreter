#ifndef FUNCTION_CALL_H
#define FUNCTION_CALL_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class FunctionCall : public Node
		{
		public:
			std::string identifier;
			std::unique_ptr<CallOperator> callOperator;
		};

	}
}

#endif // !FUNCTION_CALL_H