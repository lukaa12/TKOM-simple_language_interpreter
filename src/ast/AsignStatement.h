#ifndef ASIGN_STATEMENT_H
#define ASIGN_STATEMENT_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class AssignStatement : public Instruction
		{
		public:
			std::string identifier;
			std::unique_ptr<RightValue> rvalue;

			Type getType()
			{
				return Type::Asign;
			}
		};

	}
}

#endif // !ASIGN_STATEMENT_H
