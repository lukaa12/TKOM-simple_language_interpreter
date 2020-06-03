#ifndef BODY_H
#define BODY_H

#include "AstNode.h"
#include "../stdlib/DataTypes.h"

namespace tkom {
	namespace ast {

		class Body : public Node
		{
		public:
			void addInstruction(std::unique_ptr<Instruction> ptr)
			{
				ptr->parent = this;
				instructions.push_back(std::move(ptr));
			}

			std::vector<std::unique_ptr<Instruction>>& getInstructions()
			{
				return instructions;
			}

			DataType exec();

			std::variant<int, std::string, lib::Color, lib::Graphic> returned;
			bool wasBreak;
			bool wasReturn;

		private:
			std::vector<std::unique_ptr<Instruction>> instructions;
		};

	}
}

#endif // !BODY_H