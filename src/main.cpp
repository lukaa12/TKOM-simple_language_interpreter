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

	lib::Graphic blank = lib::blank(800, 600);
	lib::Graphic triangle = lib::triangle(100, 100);
	triangle = lib::translate(triangle, 100, 0);
	triangle = lib::setColor(triangle, lib::colorRGB(255, 0, 0));
	triangle = lib::add(triangle, triangle);
	triangle = lib::translate(triangle, 0, 100);
	blank = lib::add(blank, triangle);
	blank = lib::add(blank, lib::circle(30));

	lib::show(blank);

	return 0;
}

