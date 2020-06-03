#include "Reader.h"
#include <iostream>
#include "Error.h"
#include <string>

using namespace tkom;

Reader::Reader(std::istream& _stream): stream(_stream), position({1, 0})
{
	if (this->stream.fail())
		throw Error("Input stream error!");
}

const char Reader::next()
{
	char actual = this->stream.get();
	if (this->stream.eof())
		return '\0';
	++position.column;
	if (actual == '\n')
	{
		++this->position.line;
		this->position.column = 0;
	}
	if (actual == '\r')
	{
		++this->position.line;
		this->position.column = 0;
		if ((actual = this->stream.get()) != '\n')
		{
			throw Error("Format not supported");
		}
	}
	return actual;
}

const char Reader::peek()
{
	return this->stream.peek() == EOF ? '\0' : this->stream.peek();
}

Token::Position Reader::getPosition()
{
	return this->position;
}
