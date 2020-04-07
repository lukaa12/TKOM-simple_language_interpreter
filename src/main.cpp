#include <iostream>
#include "Reader.h"

using namespace tkom;

int main()
{
	std::cout << "START" << std::endl;
	FileReader reader{ "test.txt" };
	Reader& in = reader;
	char c = in.next();
	while (c != '\0')
	{
		std::cout << c << std::endl;
		c = in.next();
	}
	std::cout << "END" << std::endl;

	return 0;
}