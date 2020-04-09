#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace tkom {

	class Token
	{
	public:
		enum class Type
		{
			Eof,
			Invalid,
			DataType,
			Identifier,
			IntLiteral,
			StringLiteral,
			Less,
			Greater,
			LessOrEqual,
			GreaterOrEqual,
			Equal,
			NotEqual,
			Negation,
			BracketOpen,
			BracketClose,
			CurlyOpen,
			CurlyClose,
			Comma,
			Semicoln,
			If,
			While,
			Else,
			Return,
			Break,
			Assigment,
			Or,
			And,
			Plus,
			Minus,
			Multiply,
			Divide
		};

		Type type;
		union Value {
			int i;
			std::string s;

			Value(): s("")
			{}
			~Value()
			{}
		} value;
		unsigned int line;
		unsigned int column;

		Token();
		//Token(const Token& other);
		Token(const Type& _type);
		Token(const Type& _type, const std::string& _str);
		Token(const int& value);
	};

}

#endif // !TOKEN_H
