#ifndef WHILE_LOOP_H
#define WHILE_LOOP_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class WhileLoop : public Instruction
		{
		public:
			WhileLoop() = default;
			
			void setCondition(std::unique_ptr<Condition> ptr)
			{
				ptr->parent = this;
				condition = std::move(ptr);
			}

			void setBody(std::unique_ptr<Body> ptr)
			{
				ptr->parent = this;
				this->whileBody = std::move(ptr);
			}

			Condition* getCondition()
			{
				return condition.get();
			}

			Body* getBody()
			{
				return whileBody.get();
			}

			Type getType()
			{
				return Type::While;
			}
		private:
			std::unique_ptr<Condition> condition;
			std::unique_ptr<Body> whileBody;
		};

	}
}

#endif // !WHILE_LOOP_H