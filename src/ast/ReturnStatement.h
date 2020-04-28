#ifndef RETURN_STATEMENT_H
#define RETURN_STATEMENT_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class ReturnStatement : public Instruction
		{
		public:
			std::unique_ptr<RightValue> value;

			Type getType()
			{
				return Type::Return;
			}
		};

	}
}

#endif // !RETURN_STATEMENT_H