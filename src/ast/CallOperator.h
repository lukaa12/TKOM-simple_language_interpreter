#ifndef CALL_OPERATOR_H
#define CALL_OPERATOR_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class CallOperator : public Node
		{
		public:
			std::vector<std::unique_ptr<RightValue>> arguments;
		};

	}
}

#endif // !CALL_OPERATOR_H