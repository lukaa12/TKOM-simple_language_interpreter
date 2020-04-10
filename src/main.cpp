#include <iostream>
#include "Reader.h"
#include "Scanner.h"

using namespace tkom;

int main()
{
	std::cout << "START" << std::endl;
	FileReader reader{ "test.txt" };
	Reader& in = reader;
	Scanner scanner{ in };
	//char c = in.next();
	//while (c != '\0')
	//{
	//	std::cout << c << std::endl;
	//	c = in.next();
	//}
	for (auto i = 0; i < 3; ++i)
	{
		scanner.nextToken();
	}
	std::cout << "END" << std::endl;

	return 0;
}