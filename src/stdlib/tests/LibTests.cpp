#include <boost/test/auto_unit_test.hpp>
#include <string>
#include <iostream>
#include <memory>
#include "../../Parser.h"
#include "../../Executor.h"
#include "../../Error.h"

using namespace tkom;
using namespace ast;

BOOST_AUTO_TEST_SUITE(Data_types_tests)

class ExecutionFix
{
public:

	std::unique_ptr<ast::Program> getProgram(std::string prog)
	{
		in.reset(new std::stringstream(prog));
		reader.reset(new Reader(*in));
		scanner.reset(new Scanner(*reader));
		parser.reset(new Parser(*scanner));

		return parser->parse();
	}

	SymbolTable& getSymbolTable()
	{
		return parser->getTable();
	}

	std::unique_ptr<std::stringstream> in;
	std::unique_ptr<Reader> reader;
	std::unique_ptr<Scanner> scanner;
	std::unique_ptr<Parser> parser;
};

BOOST_FIXTURE_TEST_CASE(Color_type_test, ExecutionFix)
{
	auto program = getProgram(R"(
int main() 
{
	color col1;
	
	return 0;
}
)");

	BOOST_CHECK_NO_THROW(program->exec());

}

BOOST_FIXTURE_TEST_CASE(Graphic_type_test, ExecutionFix)
{
	auto program = getProgram(R"(
int main() 
{
	graphic circle;
	
	return 0;
}
)");

	BOOST_CHECK_NO_THROW(program->exec());

}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Library_functions_tests)

class ExecutionFix
{
public:

	std::unique_ptr<ast::Program> getProgram(std::string prog)
	{
		in.reset(new std::stringstream(prog));
		reader.reset(new Reader(*in));
		scanner.reset(new Scanner(*reader));
		parser.reset(new Parser(*scanner));

		return parser->parse();
	}

	SymbolTable& getSymbolTable()
	{
		return parser->getTable();
	}

	std::unique_ptr<std::stringstream> in;
	std::unique_ptr<Reader> reader;
	std::unique_ptr<Scanner> scanner;
	std::unique_ptr<Parser> parser;
};

BOOST_FIXTURE_TEST_CASE(Print_test, ExecutionFix)
{
	auto program = getProgram(R"(
int main() 
{
	print("Hello World!");
	
	return 0;
}
)");

	BOOST_CHECK_NO_THROW(program->exec());
}

BOOST_FIXTURE_TEST_CASE(Blank_test, ExecutionFix)
{
	auto program = getProgram(R"(
int main() 
{
	graphic root = blank(800, 600);
	return 0;
}
)");

	BOOST_CHECK_NO_THROW(program->exec());
}

BOOST_AUTO_TEST_CASE(Blank_test2)
{
	lib::Graphic test = lib::blank(1000, 750);

	BOOST_CHECK_EQUAL(test.vertices.size(), 0);
	BOOST_CHECK_EQUAL(test.indices.size(), 0);
	BOOST_CHECK_EQUAL(test.width, 1000);
	BOOST_CHECK_EQUAL(test.height, 750);
	BOOST_CHECK_EQUAL(test.color.color.y, 1.0f);
	BOOST_CHECK_EQUAL(test.color.color.y, 1.0f);
	BOOST_CHECK_EQUAL(test.color.color.z, 1.0f);
}

BOOST_FIXTURE_TEST_CASE(Triangle_test, ExecutionFix)
{
	auto program = getProgram(R"(
int main() 
{
	graphic test = triangle(80, 60);
	return 0;
}
)");

	BOOST_CHECK_NO_THROW(program->exec());
}

BOOST_AUTO_TEST_CASE(Triangle_test2)
{
	lib::Graphic test = lib::triangle(100, 75);

	BOOST_CHECK_EQUAL(test.vertices.size(), 6);
	BOOST_CHECK_EQUAL(test.indices.size(), 3);
}

BOOST_FIXTURE_TEST_CASE(Rectangular_triangle_test, ExecutionFix)
{
	auto program = getProgram(R"(
int main() 
{
	graphic test = rectangular_triangle(80, 60);
	return 0;
}
)");

	BOOST_CHECK_NO_THROW(program->exec());
}

BOOST_AUTO_TEST_CASE(Rectangular_triangle_test2)
{
	lib::Graphic test = lib::rectangularTriangle(100, 75);

	BOOST_CHECK_EQUAL(test.vertices.size(), 6);
	BOOST_CHECK_EQUAL(test.indices.size(), 3);
}

BOOST_FIXTURE_TEST_CASE(Rectangle_test, ExecutionFix)
{
	auto program = getProgram(R"(
int main() 
{
	graphic test = rectangle(80, 60);
	return 0;
}
)");

	BOOST_CHECK_NO_THROW(program->exec());
}

BOOST_AUTO_TEST_CASE(Rectangle_test2)
{
	lib::Graphic test = lib::rectangle(100, 75);

	BOOST_CHECK_EQUAL(test.vertices.size(), 8);
	BOOST_CHECK_EQUAL(test.indices.size(), 6);
}

BOOST_FIXTURE_TEST_CASE(Circle_test, ExecutionFix)
{
	auto program = getProgram(R"(
int main() 
{
	graphic test = circle(80);
	return 0;
}
)");

	BOOST_CHECK_NO_THROW(program->exec());
}

BOOST_AUTO_TEST_CASE(Circle_test2)
{
	lib::Graphic test = lib::circle(100);

	BOOST_CHECK_EQUAL(test.vertices.size(), 37 * 2);
}

BOOST_FIXTURE_TEST_CASE(Line_test, ExecutionFix)
{
	auto program = getProgram(R"(
int main() 
{
	graphic test = line(800);
	return 0;
}
)");

	BOOST_CHECK_NO_THROW(program->exec());
}

BOOST_AUTO_TEST_CASE(Line_test2)
{
	lib::Graphic test = lib::line(100);

	BOOST_CHECK_EQUAL(test.vertices.size(), 8);
	BOOST_CHECK_EQUAL(test.indices.size(), 6);
}

BOOST_FIXTURE_TEST_CASE(Add_test, ExecutionFix)
{
	auto program = getProgram(R"(
int main() 
{
	graphic test = blank(800, 400), test2 = circle(200);
	test = add(test, test2);
	return 0;
}
)");

	BOOST_CHECK_NO_THROW(program->exec());
}

BOOST_AUTO_TEST_CASE(Add_test2)
{
	lib::Graphic test = lib::line(100);
	test = lib::add(test, lib::circle(200));

	BOOST_CHECK_EQUAL(test.children.size(), 1);
}

BOOST_FIXTURE_TEST_CASE(Translate_test, ExecutionFix)
{
	auto program = getProgram(R"(
int main() 
{
	graphic test = circle(200);
	test = translate(test, 100, 200);
	return 0;
}
)");

	BOOST_CHECK_NO_THROW(program->exec());
}

BOOST_FIXTURE_TEST_CASE(Scale_test, ExecutionFix)
{
	auto program = getProgram(R"(
int main() 
{
	graphic test = circle(200);
	test = scale(test, 110, 200);
	return 0;
}
)");

	BOOST_CHECK_NO_THROW(program->exec());
}

BOOST_FIXTURE_TEST_CASE(Rotate_test, ExecutionFix)
{
	auto program = getProgram(R"(
int main() 
{
	graphic test = circle(200);
	test = rotate(test, 90);
	return 0;
}
)");

	BOOST_CHECK_NO_THROW(program->exec());
}

BOOST_FIXTURE_TEST_CASE(Unfill_test, ExecutionFix)
{
	auto program = getProgram(R"(
int main() 
{
	graphic test = circle(200);
	test = unfill(test);
	return 0;
}
)");

	BOOST_CHECK_NO_THROW(program->exec());
}

BOOST_FIXTURE_TEST_CASE(Color_RGB_test, ExecutionFix)
{
	auto program = getProgram(R"(
int main() 
{
	color col = color_rgb(255, 0, 127);
	
	return 0;
}
)");

	BOOST_CHECK_NO_THROW(program->exec());
}

BOOST_AUTO_TEST_CASE(Color_RGB_test2)
{
	lib::Color col = lib::colorRGB(255, 0, 0);

	BOOST_CHECK_EQUAL(col.color.x, 1.0f);
	BOOST_CHECK_EQUAL(col.color.y, 0.0f);
	BOOST_CHECK_EQUAL(col.color.z, 0.0f);
}

BOOST_FIXTURE_TEST_CASE(Set_color_test, ExecutionFix)
{
	auto program = getProgram(R"(
int main() 
{
	color col = color_rgb(255, 0, 127);
	graphic obj;
	obj = set_color(obj, col);
	
	return 0;
}
)");

	BOOST_CHECK_NO_THROW(program->exec());
}

BOOST_AUTO_TEST_CASE(Set_color_test2)
{
	lib::Color col = lib::colorRGB(255, 0, 0);
	lib::Graphic g;
	g = lib::setColor(g, col);

	BOOST_CHECK_EQUAL(g.color.color.x, 1.0f);
	BOOST_CHECK_EQUAL(g.color.color.y, 0.0f);
	BOOST_CHECK_EQUAL(g.color.color.z, 0.0f);
}

BOOST_FIXTURE_TEST_CASE(Function_as_rvalue_test, ExecutionFix)
{
	auto program = getProgram(R"(
graphic tree(int levels)
{
	graphic base = rectangle(50, 100);
	color c2 = color_rgb(10, 250, 20);
	base = set_color(base, color_rgb(210, 105, 30));
	graphic level = triangle(120, 120);
	level = set_color(level, c2);
	while (levels > 0)
	{
		level = translate(level, 0, 50);
		base = add(base, level);
		levels = levels - 1;
	}
	return base;
}

int main()
{
	graphic root = blank(800, 600);
	graphic tree1 = tree(3);
	root = add(root, tree1);
	//show(root);
	return 0;
}
)");

	BOOST_CHECK_NO_THROW(program->exec());
}

BOOST_AUTO_TEST_SUITE_END()