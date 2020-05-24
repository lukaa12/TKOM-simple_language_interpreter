#include <iostream>
#include <sstream>
#include "Reader.h"
#include "Scanner.h"
#include "Parser.h"
#include "Error.h"
#include "stdlib/Functions.h"

using namespace tkom;

int main()
{
	std::stringstream in{ R"(
int main() 
{
	print("Hello World!");
	graphic root = blank(800, 500);
	graphic triangle = triangle(200, 200);
	root = add(root, triangle);

	show(root);
	
	return 0;
}
)" };
	Reader reader{ in };
	Scanner scanner{ reader };
	Parser parser{ scanner };

	std::unique_ptr<ast::Program> program;
	int returned;

	try
	{
		program = parser.parse();
		returned = program->exec();
		std::cout << "Program returned: " << returned << std::endl;
	}
	catch (const Error& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}

