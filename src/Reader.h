#ifndef READER_H
#define READER_H

#include <fstream>
#include <string>

namespace tkom {


	class Reader 
	{
	public:
		virtual const char next() = 0;
		virtual const char peek() = 0;
		virtual const unsigned int getLine() = 0;
		virtual const unsigned int getCol() = 0;
	};

	class FileReader : public Reader 
	{
	public:
		FileReader(const std::string& _file);
		const char next();
		const char peek();
		const unsigned int getLine();
		const unsigned int getCol();
	private:
		std::ifstream file;
		unsigned int line;
		unsigned int column;
	};

}

#endif // !READER_H

