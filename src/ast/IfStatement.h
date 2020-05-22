#ifndef IF_STATEMENT_H
#define IF_STATEMENT_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class IfStatement : public Instruction
		{
		public:
			Condition* getCondition()
			{
				return condition.get();
			}

			Body* getIfBody()
			{
				return ifBody.get();
			}

			Body* getElseBody()
			{
				return elseBody.get();
			}

			void setCondition(std::unique_ptr<Condition> ptr)
			{
				ptr->parent = this;
				this->condition = std::move(ptr);
			}

			void setIfBody(std::unique_ptr<Body> ptr)
			{
				ptr->parent = this;
				this->ifBody = std::move(ptr);
			}

			void setElseBody(std::unique_ptr<Body> ptr)
			{
				ptr->parent = this;
				this->elseBody = std::move(ptr);
			}

			Type getType()
			{
				return Type::If;
			}

			void exec();
			bool wasReturned;
			bool wasBreaked;
			DataType dtype;
			std::variant<int, std::string> returned;

		private:
			std::unique_ptr<Condition> condition;
			std::unique_ptr<Body> ifBody;
			std::unique_ptr<Body> elseBody;
		};

	}
}

#endif // !IF_STATEMENT_H