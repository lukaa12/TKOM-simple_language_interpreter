#ifndef FUNCTION_CALL_H
#define FUNCTION_CALL_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class FunctionCall : public Node
		{
		public:
			std::string getIdentifier()
			{
				return identifier;
			}

			void setIdentifier(std::string id)
			{
				identifier = id;
			}

			std::shared_ptr<CallOperator> getCallOperator()
			{
				return callOperator;
			}

			void setCallOperator(const std::shared_ptr<CallOperator>& ptr)
			{
				ptr->parent = std::make_shared<Node>(*this);
				callOperator = ptr;
			}

		private:
			std::string identifier;
			std::shared_ptr<CallOperator> callOperator;
		};

	}
}

#endif // !FUNCTION_CALL_H