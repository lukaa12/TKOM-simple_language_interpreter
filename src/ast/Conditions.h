#ifndef CONDITIONS_H
#define CONDITIONS_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class Condition;

		class BracesCondition : public Node
		{
		public:
			BracesCondition() = default;

			BracesCondition(std::unique_ptr<Condition> ptr): condition(std::move(ptr))
			{}

			void setCondition(std::unique_ptr<Condition> ptr);

			Condition* getCondition()
			{
				return condition.get();
			}

		private:
			std::unique_ptr<Condition> condition;
		};

		class PrimaryCondition : public Node
		{
		public:
			enum class Type {
				RightVal,
				Bracket
			};

			PrimaryCondition() = default;

			PrimaryCondition(Type t, bool neg, std::unique_ptr<RightValue> ptr): type(t), negated(neg), 
				condition(std::move(ptr))
			{}

			PrimaryCondition(Type t, bool neg, std::unique_ptr<BracesCondition> ptr) : type(t), negated(neg),
				condition(std::move(ptr))
			{}

			void setType(Type t)
			{
				type = t;
			}

			Type getType() const
			{
				return type;
			}

			void setNegated(bool is)
			{
				negated = is;
			}

			bool isNegated() const
			{
				return negated;
			}

			int eval(); // interpreting ints as conditions like in C

			template<typename T>
			T* getCondition();

			template<typename T>
			void setCondition(T val);

		private: 
			Type type;
			bool negated;
			std::variant<std::unique_ptr<RightValue>, std::unique_ptr<BracesCondition>> condition;
		};

		class RelationCondition : public Node
		{
		public:
			RelationCondition() = default;

			RelationCondition(std::unique_ptr<PrimaryCondition> _first, std::unique_ptr<PrimaryCondition> _second,
				const RelationOperator& op): first(std::move(_first)), second(std::move(_second)), relationOper(op)
			{}

			PrimaryCondition* getFirst()
			{
				return first.get();
			}

			void setFirst(std::unique_ptr<PrimaryCondition> ptr)
			{
				ptr->parent = this;
				first = std::move(ptr);
			}

			PrimaryCondition* getSecond()
			{
				return second.get();
			}

			void setSecond(std::unique_ptr<PrimaryCondition> ptr)
			{
				ptr->parent = this;
				second = std::move(ptr);
			}

			void setRelationOper(RelationOperator op)
			{
				relationOper = op;
			}

			RelationOperator getRelationOper()
			{
				return relationOper;
			}

			int eval();

		private:
			std::unique_ptr<PrimaryCondition> first;
			RelationOperator relationOper;
			std::unique_ptr<PrimaryCondition> second;
		};

		class AndCondition : public Node
		{
		public:
			std::vector<std::unique_ptr<RelationCondition>>& getComponents()
			{
				return relationConditions;
			}

			void addComponent(std::unique_ptr<RelationCondition> ptr)
			{
				ptr->parent = this;
				relationConditions.push_back(std::move(ptr));
			}

			int eval();

		private:
			std::vector<std::unique_ptr<RelationCondition>> relationConditions;
		};

		class Condition : public Node
		{
		public:
			std::vector<std::unique_ptr<AndCondition>>& getComponents()
			{
				return andConditions;
			}

			void addComponent(std::unique_ptr<AndCondition> ptr)
			{
				ptr->parent = this;
				andConditions.push_back(std::move(ptr));
			}

			int eval();

		private:
			std::vector<std::unique_ptr<AndCondition>> andConditions;
		};

		template<typename T>
		T* PrimaryCondition::getCondition()
		{
			return std::get<std::unique_ptr<T>>(condition).get();
		}

		template<typename T>
		void PrimaryCondition::setCondition(T val)
		{
			val->parent = this;
			condition = std::move(val);
		}

	}
}

#endif // !CONDITIONS_H