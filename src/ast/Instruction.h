#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class Instruction : public Node
		{
		public:
			enum class Type {
				If,
				While,
				Assign,
				Init,
				FuncExec,
				Return,
				Break
			};
			virtual Type getType()
			{
				return Type::Break;
			}

			virtual ~Instruction()
			{}

			virtual void exec();
		};

	}
}

#endif // !INSTRUCTION_H