#include <iostream>
#include <fstream>
#include "Reader.h"
#include "Scanner.h"
#include "Parser.h"
#include "Error.h"

using namespace tkom;

int main(int argc, const char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: interpret <path_to_source>" << std::endl;
		return 2;
	}

	std::ifstream inputFile;

	inputFile.open(argv[1]);
	if (!inputFile) 
	{
		std::cerr << "Unable to open file";
		return 1;
	}
	
	Reader reader{ inputFile };
	Scanner scanner{ reader };
	Parser parser{ scanner };

	std::unique_ptr<ast::Program> program;

	try
	{
		program = parser.parse();
		int returned = program->exec();
		std::cout << "Program returned: " << returned << std::endl;
	}
	catch (const Error& e)
	{
		std::cout << e.what() << std::endl;
	}
	inputFile.close();

	return 0;
}

