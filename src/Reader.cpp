#include "Reader.h"
#include <iostream>
#include <exception>
#include <string>

using namespace tkom;

FileReader::FileReader(const std::string& _file)
	: file(_file), line(1), column(0)
{
	if (!this->file.is_open() || this->file.fail())
	{
		throw std::exception(std::string("Cannot open file: ").append(_file).c_str());
	}
}

const char FileReader::next()
{
	char actual = this->file.get();
	if (this->file.eof())
		return '\0';
	++column;
	if (actual == '\n')
	{
		++this->line;
		this->column = 0;
	}
	if (actual == '\r')
	{
		++this->line;
		this->column = 0;
		if ((actual = this->file.get()) != '\n')
		{
			std::exception("File format not supported");
		}
	}
	return actual;
}

const char FileReader::peek()
{
	return this->file.peek() == EOF ? '\0' : this->file.peek();
}
const unsigned int FileReader::getLine()
{
	return this->line;
}
const unsigned int FileReader::getCol() 
{
	return this->column;
}

TestReader::TestReader(const std::string& str): stream(str), line(1), column(0)
{}

const char TestReader::next()
{
	return stream.get();
}

const char TestReader::peek()
{
	return stream.peek();
}

const unsigned int TestReader::getLine()
{
	return line;
}
const unsigned int TestReader::getCol()
{
	return column;
}