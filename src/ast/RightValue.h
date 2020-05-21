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
			T* getValue()
			{
				return std::get<std::unique_ptr<T>>(value).get();
			}

			template<typename T>
			T getValue(int)
			{
				return std::get<T>(value);
			}

			template<typename T>
			void setValue(T val)
			{
				if (typeid(val) == typeid(std::unique_ptr<FunctionCall>) || typeid(val) == typeid(std::unique_ptr<Expression>))
					val->parent = this;
				value = std::move(val);
			}

			template<>
			void setValue(std::string val)
			{
				value = val;
			}

			template<>
			void setValue(int val)
			{
				value = val;
			}

			DataType getDataType();
			void exec();
			int intValue();
			std::string stringValue();

		private:
			Type type;
			std::variant<int, std::string, std::unique_ptr<FunctionCall>, std::unique_ptr<Expression>> value;
		};

	}
}

#endif // !RIGHT_VALUE_H