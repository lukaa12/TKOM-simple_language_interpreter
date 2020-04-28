#ifndef IF_STATEMENT_H
#define IF_STATEMENT_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class IfStatement : public Instruction
		{
		public:
			std::unique_ptr<Condition> condition;
			std::unique_ptr<Body> ifBody;
			std::unique_ptr<Body> elseBody;

			Type getType()
			{
				return Type::If;
			}
		};

	}
}

#endif // !IF_STATEMENT_H