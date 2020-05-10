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

			CallDef* getCallDef()
			{
				return requiredArguments.get();
			}

			void setCallDef(std::unique_ptr<CallDef> ptr)
			{
				ptr->parent = this;
				requiredArguments = std::move(ptr);
			}

			Body* getFunctionBody()
			{
				return functionBody.get();
			}

			void setFunctionBody(std::unique_ptr<Body> ptr)
			{
				ptr->parent = this;
				functionBody = std::move(ptr);
			}

		private:
			DataType returnType;
			std::string identifier;
			std::unique_ptr<CallDef> requiredArguments;
			std::unique_ptr<Body> functionBody;
		};

	}
}

#endif // !FUNCTION_DEF_H