#ifndef RIGHT_VALUE_H
#define RIGHT_VALUE_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class RightValue : public Node
		{
		public:
			enum class Type {
				StringLiteral,
				IntLiteral,
				Identifier,
				Function,
				Expression
			} type;
			std::variant<int, std::string, std::unique_ptr<FunctionCall>, std::unique_ptr<Expression>> value;
		};

	}
}

#endif // !RIGHT_VALUE_H