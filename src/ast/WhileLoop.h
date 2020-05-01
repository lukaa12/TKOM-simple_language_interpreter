#ifndef WHILE_LOOP_H
#define WHILE_LOOP_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class WhileLoop : public Instruction
		{
		public:
			WhileLoop() = default;
			
			void setCondition(const std::shared_ptr<Condition>& ptr)
			{
				ptr->parent = std::make_shared<Node>(*this);
				condition = ptr;
			}

			void setBody(const std::shared_ptr<Body>& ptr)
			{
				ptr->parent = std::make_shared<Node>(*this);
				this->whileBody = ptr;
			}

			std::shared_ptr<Condition> getCondition()
			{
				return condition;
			}

			std::shared_ptr<Body> getBody()
			{
				return whileBody;
			}

			Type getType()
			{
				return Type::While;
			}
		private:
			std::shared_ptr<Condition> condition;
			std::shared_ptr<Body> whileBody;
		};

	}
}

#endif // !WHILE_LOOP_H