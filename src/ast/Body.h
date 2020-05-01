#ifndef BODY_H
#define BODY_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class Body : public Node
		{
		public:
			void addInstruction(const std::shared_ptr<Instruction>& ptr)
			{
				ptr->parent = std::make_shared<Node>(*this);
				instructions.push_back(ptr);
			}

			std::vector<std::shared_ptr<Instruction>>& getInstructions()
			{
				return instructions;
			}

		private:
			std::vector<std::shared_ptr<Instruction>> instructions;
		};

	}
}

#endif // !BODY_H