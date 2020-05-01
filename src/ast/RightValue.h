#ifndef RIGHT_VALUE_H
#define RIGHT_VALUE_H

#include "AstNode.h"
#include <typeinfo>

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
			};

			Type getType()
			{
				return type;
			}

			void setType(Type t)
			{
				type = t;
			}

			template<typename T>
			T getValue()
			{
				return std::get<T>(value);
			}

			template<typename T>
			void setValue(T& val)
			{
				if (typeid(val) == typeid(std::shared_ptr<FunctionCall>) || typeid(val) == typeid(std::shared_ptr<Expression>))
					val->parent = std::make_shared<Node>(*this);
				value = val;
			}

		private:
			Type type;
			std::variant<int, std::string, std::shared_ptr<FunctionCall>, std::shared_ptr<Expression>> value;
		};

	}
}

#endif // !RIGHT_VALUE_H