#ifndef CONDITIONS_H
#define CONDITIONS_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		enum class RelationOperator {
			Less,
			LessEqual,
			Equal,
			GreaterEqual,
			Greater,
			NotEqual
		};

		class BracesCondition;
		class PrimaryCondition;
		class RelationCondition;
		class AndCondition;

		class Condition : public Node
		{
		public:
			std::vector<std::unique_ptr<AndCondition>> andConditions;
		};

		class AndCondition : public Node
		{
		public:
			std::vector<std::unique_ptr<RelationCondition>> relationConditions;
		};

		class RelationCondition : public Node
		{
		public:
			std::unique_ptr<PrimaryCondition> first;
			RelationOperator relationOper;
			std::unique_ptr<PrimaryCondition> second;
		};

		class PrimaryCondition : public Node
		{
		public:
			enum class Type {
				RightVal,
				Bracket
			} type;
			bool negated;
			std::variant<std::unique_ptr<RightValue>, std::unique_ptr<BracesCondition>> condition;
		};

		class BracesCondition : public Node
		{
		public:
			std::unique_ptr<Condition> condition;
		};

		std::string toString(const RelationOperator& op)
		{
			switch (op)
			{
			case RelationOperator::Less:
				return "<";
			case RelationOperator::LessEqual:
				return "<=";
			case RelationOperator::Equal:
				return "==";
			case RelationOperator::GreaterEqual:
				return ">=";
			case RelationOperator::Greater:
				return ">";
			case RelationOperator::NotEqual:
				return "!=";
			}
		}

	}
}

#endif // !CONDITIONS_H