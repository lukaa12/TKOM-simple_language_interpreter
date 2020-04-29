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
			Semicolon,
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

		struct Position
		{
			unsigned int line;
			unsigned int column;
		};
		
		Token(Position _pos = Position{ 0, 0 });
		Token(Type type, Position _pos);
		Token(Type type, Position _pos, int val);
		Token(Type type, Position _pos, std::string val);
		Token(const Token& other);
		Token& operator=(const Token& other);
		Type getType() const;
		int getIntVal() const;
		std::string getStrVal() const;
		Position getPosition() const;


		static std::string toString(Token::Type type);
	private:
		Type type;
		int int_value;
		std::string s_value;
		Position position;
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

	static const std::unordered_map<char, Token::Type>& simpleSigns = {
		{ '{', Token::Type::CurlyOpen },
		{ '}', Token::Type::CurlyClose },
		{ '(', Token::Type::BracketOpen },
		{ ')', Token::Type::BracketClose },
		{ ',', Token::Type::Comma },
		{ ';', Token::Type::Semicolon },
		{ '+', Token::Type::Plus },
		{ '-', Token::Type::Minus },
		{ '*', Token::Type::Multiply },
	};

	inline std::ostream& operator<<(std::ostream& o, Token::Type type)
	{
		return o << Token::toString(type);
	}
}

#endif // !TOKEN_H
