#ifndef CONDITIONS_H
#define CONDITIONS_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class Condition;

		class BracesCondition : public Node
		{
		public:
			void setCondition(std::shared_ptr<Condition>& ptr);

			std::shared_ptr<Condition> getCondition()
			{
				return condition;
			}

		private:
			std::shared_ptr<Condition> condition;
		};

		class PrimaryCondition : public Node
		{
		public:
			enum class Type {
				RightVal,
				Bracket
			};

			void setType(Type t)
			{
				type = t;
			}

			Type getType()
			{
				return type;
			}

			void setNegated(bool is)
			{
				negated = is;
			}

			bool isNegated()
			{
				return negated;
			}

			template<typename T>
			T getCondition();

			template<typename T>
			void setCondition(T val);

		private: 
			Type type;
			bool negated;
			std::variant<std::shared_ptr<RightValue>, std::shared_ptr<BracesCondition>> condition;
		};

		class RelationCondition : public Node
		{
		public:
			std::shared_ptr<PrimaryCondition> getFirst()
			{
				return first;
			}

			void setFirst(std::shared_ptr<PrimaryCondition>& ptr)
			{
				ptr->parent = std::make_shared<Node>(*this);
				first = ptr;
			}

			std::shared_ptr<PrimaryCondition> getSecond()
			{
				return second;
			}

			void setSecond(std::shared_ptr<PrimaryCondition>& ptr)
			{
				ptr->parent = std::make_shared<Node>(*this);
				second = ptr;
			}

			void setRelationOper(RelationOperator op)
			{
				relationOper = op;
			}

			RelationOperator getRelationOper()
			{
				return relationOper;
			}

		private:
			std::shared_ptr<PrimaryCondition> first;
			RelationOperator relationOper;
			std::shared_ptr<PrimaryCondition> second;
		};

		class AndCondition : public Node
		{
		public:
			std::vector<std::shared_ptr<RelationCondition>>& getComponents()
			{
				return relationConditions;
			}

			void addComponent(std::shared_ptr<RelationCondition>& ptr)
			{
				ptr->parent = std::make_shared<Node>(*this);
				relationConditions.push_back(ptr);
			}
		private:
			std::vector<std::shared_ptr<RelationCondition>> relationConditions;
		};

		class Condition : public Node
		{
		public:
			std::vector<std::shared_ptr<AndCondition>>& getComponents()
			{
				return andConditions;
			}

			void addComponent(std::shared_ptr<AndCondition>& ptr)
			{
				ptr->parent = std::make_shared<Node>(*this);
				andConditions.push_back(ptr);
			}

		private:
			std::vector<std::shared_ptr<AndCondition>> andConditions;
		};

		template<typename T>
		T PrimaryCondition::getCondition()
		{
			return std::get<T>(condition);
		}

		template<typename T>
		void PrimaryCondition::setCondition(T val)
		{
			val->parent = std::make_shared<Node>(*this);
			condition = val;
		}

	}
}

#endif // !CONDITIONS_H