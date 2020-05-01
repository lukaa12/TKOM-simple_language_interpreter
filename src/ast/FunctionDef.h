#ifndef FUNCTION_DEF_H
#define FUNCTION_DEF_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class FunctionDef : public Node
		{
		public:
			DataType getReturnType()
			{
				return returnType;
			}

			void setReturnType(DataType type)
			{
				returnType = type;
			}

			std::string getIdentifier()
			{
				return identifier;
			}

			void setIdentifier(std::string id)
			{
				identifier = id;
			}

			std::shared_ptr<CallDef> getCallDef()
			{
				return requiredArguments;
			}

			void setCallDef(const std::shared_ptr<CallDef>& ptr)
			{
				ptr->parent = std::make_shared<Node>(*this);
				requiredArguments = ptr;
			}

			std::shared_ptr<Body> getFunctionBody()
			{
				return functionBody;
			}

			void seyFunctionBody(const std::shared_ptr<Body>& ptr)
			{
				ptr->parent = std::make_shared<Node>(*this);
				functionBody = ptr;
			}

		private:
			DataType returnType;
			std::string identifier;
			std::shared_ptr<CallDef> requiredArguments;
			std::shared_ptr<Body> functionBody;
		};

	}
}

#endif // !FUNCTION_DEF_H