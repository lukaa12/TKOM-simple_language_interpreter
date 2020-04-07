#include "Reader.h"
#include <iostream>
using namespace tkom;

FileReader::FileReader(const std::string& _file)
	: file(_file), line(1), column(0)
{
	if (!this->file.is_open() || this->file.fail())
	{
		//TODO
	}
}

const char FileReader::next()
{
	char actual = this->file.get();
	if (this->file.eof())
		return '\0';

	if (actual == '\n')
	{
		++this->line;
		this->column = 0;
		actual = this->file.get();
	}
	if (actual == '\r')
	{
		++this->line;
		this->column = 0;
		if (this->file.get() != '\n')
		{
			//TODO
		}
		actual = this->file.get();
	}
	return actual;
}

const char FileReader::peek()
{
	return this->file.peek();
}
const unsigned int FileReader::getLine()
{
	return this->line;
}
const unsigned int FileReader::getCol() 
{
	return this->column;
}