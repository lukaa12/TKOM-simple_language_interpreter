#ifndef RETURN_STATEMENT_H
#define RETURN_STATEMENT_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class ReturnStatement : public Instruction
		{
		public:
			ReturnStatement() = default;

			std::shared_ptr<RightValue> getValue()
			{
				return value;
			}

			void setValue(std::shared_ptr<RightValue>& ptr)
			{
				ptr->parent = std::make_shared<Node>(*this);
				this->value = ptr;
			}

			Type getType()
			{
				return Type::Return;
			}

		private:
			std::shared_ptr<RightValue> value;
		};

	}
}

#endif // !RETURN_STATEMENT_H