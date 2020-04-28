#ifndef WHILE_LOOP_H
#define WHILE_LOOP_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class WhileLoop : public Instruction
		{
		public:
			std::unique_ptr<Condition> condition;
			std::unique_ptr<Body> whileBody;

			Type getType()
			{
				return Type::While;
			}
		};

	}
}

#endif // !WHILE_LOOP_H