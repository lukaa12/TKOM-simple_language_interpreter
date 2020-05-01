#include "../Parser.h"
#include <boost/test/auto_unit_test.hpp>
#include "../Error.h"

using namespace tkom;

BOOST_AUTO_TEST_SUITE(Parser_tests)

BOOST_AUTO_TEST_CASE(Parser_initialization_test)
{
	std::stringstream in{ "int main() \n{\n\n}\nstring fun(int i)\n{\n\n}\n" };
	Reader reader{ in };
	Scanner scanner{ reader };
	Parser parser{ scanner };

	std::shared_ptr<ast::Program> program;
	BOOST_CHECK_NO_THROW(program = parser.parse());
	BOOST_CHECK_EQUAL(program->getFunctions().size(), 2);
}

BOOST_AUTO_TEST_CASE(Unexpected_token_test)
{
	std::stringstream in{ "int main() \n{\n\n}\nstringERROR fun(int i)\n{\n\n}\n" };
	Reader reader{ in };
	Scanner scanner{ reader };
	Parser parser{ scanner };

	std::shared_ptr<ast::Program> program;
	BOOST_CHECK_THROW(program = parser.parse(), Error);
}

BOOST_AUTO_TEST_SUITE_END()