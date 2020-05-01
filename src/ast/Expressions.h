#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class Expression;

		class BracketExpression : public Node
		{
		public:
			std::shared_ptr<Expression> getExpression()
			{
				return expression;
			}

			void setExpression(std::shared_ptr<Expression>& ptr);

		private:
			std::shared_ptr<Expression> expression;
		};

		class PrimaryExpression : public Node
		{
		public:
			enum class Type {
				Literal,
				Identifier,
				Function,
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

			template<typename T>
			T getValue();		

			template<typename T>
			void setValue(T val);

			template<>
			void setValue(int val);

			template<>
			void setValue(std::string val);

		private:
			Type type;
			std::variant<std::string, int, std::shared_ptr<FunctionCall>, std::shared_ptr<BracketExpression>> value;
		};

		class MultiplicativeExpression : public Node
		{
		public:
			void addComponent(std::shared_ptr<PrimaryExpression>& component)
			{
				component->parent = std::make_shared<Node>(*this);
				components.push_back(component);
				if (components.size() != divisionFlags.size() + 1)
					throw std::exception("Illegal multiplicative expression");
			}

			void addOperator(bool isDivision)
			{
				divisionFlags.push_back(isDivision);
			}

			std::vector<std::shared_ptr<PrimaryExpression>>& getComponents()
			{
				return components;
			}

			std::vector<bool>& getOperators()
			{
				return divisionFlags;
			}

		private:
			std::vector<std::shared_ptr<PrimaryExpression>> components;
			std::vector<bool> divisionFlags;
		};

		class Expression : public Node
		{
		public:
			std::vector<std::pair<bool, std::shared_ptr<MultiplicativeExpression>>>& getComponents()
			{
				return components;
			}

			void addComponent(std::pair<bool, std::shared_ptr<MultiplicativeExpression>> component)
			{
				component.second->parent = std::make_shared<Node>(*this);
				components.push_back(component);
			}

		private:
			std::vector<std::pair<bool, std::shared_ptr<MultiplicativeExpression>>> components;
		};

		template<typename T>
		T PrimaryExpression::getValue()
		{
			return std::get<T>(value);
		}

		template<typename T>
		void PrimaryExpression::setValue(T val)
		{
			if (typeid(val) == typeid(std::shared_ptr<FunctionCall>) || typeid(val) == typeid(std::shared_ptr<BracketExpression>))
				val->parent = std::make_shared<Node>(*this);
			value = val;
		}

		template<>
		void PrimaryExpression::setValue(int val)
		{
			value = val;
		}

		template<>
		void PrimaryExpression::setValue(std::string val)
		{
			value = val;
		}

	}
}

#endif // !EXPRESSIONS_H