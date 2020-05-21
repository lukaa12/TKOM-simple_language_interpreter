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

			CallOperator* getCallOperator()
			{
				return callOperator.get();
			}

			void setCallOperator(std::unique_ptr<CallOperator> ptr)
			{
				ptr->parent = this;
				callOperator = std::move(ptr);
			}

			void exec();
			int returnInt();
			std::string returnString();
			//Add other types later
			bool checkArguments();
			void assignArguments();

		private:
			std::string identifier;
			std::unique_ptr<CallOperator> callOperator;
		};

	}
}

#endif // !FUNCTION_CALL_H