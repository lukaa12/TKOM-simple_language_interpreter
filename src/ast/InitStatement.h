#ifndef INIT_STATEMENT_H
#define INIT_STATEMENT_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class InitStatement : public Instruction
		{
		public:
			DataType dataType;
			std::vector<std::pair<std::string, std::unique_ptr<RightValue>>> initiated;

			Type getType()
			{
				return Type::Init;
			}
		};

	}
}

#endif // !INIT_STATEMENT_H