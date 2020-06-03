#ifndef ASIGN_STATEMENT_H
#define ASIGN_STATEMENT_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class AssignStatement : public Instruction
		{
		public:
			std::string getIdentifier()
			{
				return identifier;
			}

			void setIdentifier(std::string str)
			{
				identifier = str;
			}

			RightValue* getRval()
			{
				return rvalue.get();
			}

			void setRval(std::unique_ptr<RightValue> ptr)
			{
				ptr->parent = this;
				rvalue = std::move(ptr);
			}

			Type getType()
			{
				return Type::Assign;
			}

			void exec();

		private:
			std::string identifier;
			std::unique_ptr<RightValue> rvalue;
		};

	}
}

#endif // !ASIGN_STATEMENT_H
