#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "AstNode.h"

namespace tkom {
	namespace ast {


		class Expression;

		class BracketExpression : public Node
		{
		public:
			Expression* getExpression()
			{
				return expression.get();
			}

			void setExpression(std::unique_ptr<Expression> ptr);

		private:
			std::unique_ptr<Expression> expression;
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
			T* getValue();

			template<typename T>
			T getValue(int);

			template<typename T>
			void setValue(T val);

			template<>
			void setValue(int val);

			template<>
			void setValue(std::string val);

			int eval();

		private:
			Type type;
			std::variant<std::string, int, std::unique_ptr<FunctionCall>, std::unique_ptr<BracketExpression>> value;
		};

		class MultiplicativeExpression : public Node
		{
		public:
			void addComponent(std::unique_ptr<PrimaryExpression> component)
			{
				component->parent = this;
				components.push_back(std::move(component));
				if (components.size() != divisionFlags.size() + 1)
					throw std::exception("Illegal multiplicative expression");
			}

			void addOperator(bool isDivision)
			{
				divisionFlags.push_back(isDivision);
			}

			std::vector<std::unique_ptr<PrimaryExpression>>& getComponents()
			{
				return components;
			}

			std::vector<bool>& getOperators()
			{
				return divisionFlags;
			}

			int eval();

		private:
			std::vector<std::unique_ptr<PrimaryExpression>> components;
			std::vector<bool> divisionFlags;
		};

		class Expression : public Node
		{
		public:
			std::vector<std::pair<bool, std::unique_ptr<MultiplicativeExpression>>>& getComponents()
			{
				return components;
			}

			void addComponent(std::pair<bool, std::unique_ptr<MultiplicativeExpression>> component)
			{
				component.second->parent = this;
				components.push_back(std::move(component));
			}

			int eval();

		private:
			std::vector<std::pair<bool, std::unique_ptr<MultiplicativeExpression>>> components;
		};

		template<typename T>
		T* PrimaryExpression::getValue()
		{
			return std::get<std::unique_ptr<T>>(value).get();
		}

		template<typename T>
		T PrimaryExpression::getValue(int)
		{
			return std::get<T>(value);
		}

		template<typename T>
		void PrimaryExpression::setValue(T val)
		{
			if (typeid(val) == typeid(std::unique_ptr<FunctionCall>) || typeid(val) == typeid(std::unique_ptr<BracketExpression>))
				val->parent = this;
			value = std::move(val);
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