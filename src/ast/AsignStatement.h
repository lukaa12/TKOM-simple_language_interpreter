#ifndef ASIGN_STATEMENT_H
#define ASIGN_STATEMENT_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class AsignStatement : public Instruction
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

			std::shared_ptr<RightValue> getRval()
			{
				return rvalue;
			}

			void setRval(std::shared_ptr<RightValue>& ptr)
			{
				ptr->parent = std::make_shared<Node>(*this);
			}

			Type getType()
			{
				return Type::Asign;
			}

		private:
			std::string identifier;
			std::shared_ptr<RightValue> rvalue;
		};

	}
}

#endif // !ASIGN_STATEMENT_H
