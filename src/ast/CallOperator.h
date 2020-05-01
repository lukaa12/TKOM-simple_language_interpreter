#ifndef CALL_OPERATOR_H
#define CALL_OPERATOR_H

#include "AstNode.h"
#include "RightValue.h"

namespace tkom {
	namespace ast {

		class CallOperator : public Node
		{
		public:

			void addArgument(const std::shared_ptr<RightValue>& ptr)
			{
				ptr->parent = std::make_shared<Node>(*this);
				arguments.push_back(ptr);
			}

			std::vector<std::shared_ptr<RightValue>>& getArguments()
			{
				return arguments;
			}

		private:
			std::vector<std::shared_ptr<RightValue>> arguments;
		};

	}
}

#endif // !CALL_OPERATOR_H