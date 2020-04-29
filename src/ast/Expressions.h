#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class BracketExpression;
		class PrimaryExpression;
		class MultiplicativeExpression;

		class Expression : public Node
		{
		public:
			std::vector<std::pair<bool, std::unique_ptr<MultiplicativeExpression>>> components;
		};

		class MultiplicativeExpression : public Node
		{
		public:
			std::vector<std::unique_ptr<PrimaryExpression>> components;
			std::vector<bool> divisionFlags;
		};

		class PrimaryExpression : public Node
		{
		public:
			enum class Type {
				Literal,
				Identifier,
				Function,
				Bracket
			} type;
			std::variant<std::string, int, std::unique_ptr<FunctionCall>, std::unique_ptr<BracketExpression>> value;
		};

		class BracketExpression : public Node
		{
		public:
			std::unique_ptr<Expression> expression;
		};

	}
}

#endif // !EXPRESSIONS_H