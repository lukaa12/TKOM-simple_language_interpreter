#ifndef ERROR_H
#define ERROR_H

#include <exception>
#include <variant>
#include "Token.h"

namespace tkom {

	class Error : public std::exception
	{
	public:
		enum class Type {
			UnexpectedToken,
			UndefinedReference,
			MissingStatement,
			IncorrectParametersList,
			MissingMain,
			UncompatibleType,
			CallOnNonFunction,
			DivisionByZero,
			BreakOutsideLoop,
			FunctionNotReturnedValue
		};
		Error(const Token::Position& pos, Type _type);
		Error(const Token& token, Type _type);
		Error();

		const char* what() const override;

	private:
		std::variant<Token, Token::Position> info;
		const Type type;
		std::string whatMessage;
	};

}

#endif //ERROR_H