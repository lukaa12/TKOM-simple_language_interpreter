#ifndef READER_H
#define READER_H

#include <istream>
#include <string>
#include "Token.h"

namespace tkom {

	class Reader 
	{
	public:
		Reader(std::istream& _stream);
		const char next();
		const char peek();
		Token::Position getPosition();
	private:
		std::istream &stream;
		Token::Position position;
	};

}

#endif // !READER_H

