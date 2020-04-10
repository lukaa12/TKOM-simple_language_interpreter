#ifndef TOKEN_H
#define TOKEN_H

#include <ostream>
#include <string>
#include <unordered_map>

namespace tkom {

	class Token
	{
	public:
		enum class Type
		{
			Eof,
			Invalid,
			IntType,
			StringType,
			GraphicType,
			ColorType,
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
		
		Token(unsigned int _line, unsigned int _column);
		Token(Type type, unsigned int _line, unsigned int _column);
		Token(Type type, unsigned int _line, unsigned int _column, int val);
		Token(Type type, unsigned int _line, unsigned int _column, std::string val);
		Token(const Token& other);
		Type getType();
		int getIntVal();
		std::string getStrVal();
		unsigned int getLine();
		unsigned int getColumn();

		static std::string toString(Token::Type type);
	private:
		const Type type;
		const int int_value;
		const std::string s_value;
		const unsigned int line;
		const unsigned int column;
	};

	static const std::unordered_map<std::string, Token::Type>& keywords = {
			{"int", Token::Type::IntType},
			{"string", Token::Type::StringType},
			{"graphic", Token::Type::GraphicType},
			{"color", Token::Type::ColorType},
			{"return", Token::Type::Return},
			{"break", Token::Type::Break},
			{"if", Token::Type::If},
			{"while", Token::Type::While},
			{"else", Token::Type::Else}
	};

	inline std::ostream& operator<<(std::ostream& o, Token::Type type)
	{
		return o << Token::toString(type);
	}
}

#endif // !TOKEN_H
