#ifndef SCANNER_H
#define SCANNER_H

#include "Reader.h"
#include "Token.h"

namespace tkom {

	class Scanner
	{
	public:
		Scanner(Reader& _reader);
		Token nextToken();
	private:
		Reader& reader;
	};
}


#endif // !SCANNER_H