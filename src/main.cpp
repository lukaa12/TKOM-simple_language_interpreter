#include <iostream>
#include <sstream>
#include "Reader.h"
#include "Scanner.h"
#include "Parser.h"
#include "Error.h"

using namespace tkom;

int main()
{
	std::stringstream in{ "//Sample script\nint main()\n{\nint i = 7;\nreturn i;\n}\n" };
	Reader reader{ in };
	Scanner scanner{ reader };
	Parser parser{ scanner };

	std::unique_ptr<ast::Program> program;

	try
	{
		program = parser.parse();
	}
	catch (const Error& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}