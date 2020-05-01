#ifndef IF_STATEMENT_H
#define IF_STATEMENT_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class IfStatement : public Instruction
		{
		public:
			std::shared_ptr<Condition> getCondition()
			{
				return condition;
			}

			std::shared_ptr<Body> getIfBody()
			{
				return ifBody;
			}

			std::shared_ptr<Body> getElseBody()
			{
				return elseBody;
			}

			void setCondition(std::shared_ptr<Condition>& ptr)
			{
				ptr->parent = std::make_shared<Node>(*this);
				this->condition = ptr;
			}

			void setIfBody(std::shared_ptr<Body>& ptr)
			{
				ptr->parent = std::make_shared<Node>(*this);
				this->ifBody = ptr;
			}

			void setElseBody(std::shared_ptr<Body>& ptr)
			{
				ptr->parent = std::make_shared<Node>(*this);
				this->elseBody = ptr;
			}

			Type getType()
			{
				return Type::If;
			}

		private:
			std::shared_ptr<Condition> condition;
			std::shared_ptr<Body> ifBody;
			std::shared_ptr<Body> elseBody;
		};

	}
}

#endif // !IF_STATEMENT_H