#ifndef RETURN_STATEMENT_H
#define RETURN_STATEMENT_H

#include "AstNode.h"
#include "../stdlib/DataTypes.h"

namespace tkom {
	namespace ast {

		class ReturnStatement : public Instruction
		{
		public:
			ReturnStatement() = default;

			RightValue* getValue()
			{
				return value.get();
			}

			void setValue(std::unique_ptr<RightValue> ptr)
			{
				ptr->parent = this;
				this->value = std::move(ptr);
			}

			Type getType()
			{
				return Type::Return;
			}

			void exec();
			DataType dtype;
			std::variant<int, std::string, lib::Color, lib::Graphic> returned;

		private:
			std::unique_ptr<RightValue> value;
		};

	}
}

#endif // !RETURN_STATEMENT_H