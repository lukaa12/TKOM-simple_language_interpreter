#ifndef CONDITIONS_H
#define CONDITIONS_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

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


	}
}

#endif // !CONDITIONS_H