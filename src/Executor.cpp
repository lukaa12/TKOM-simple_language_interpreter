#include "Executor.h"
#include "Error.h"

using namespace tkom;

SymbolTable Executor::symbolTable = SymbolTable();

Executor::Executor(ast::Program* prog) : program(prog)
{

}

void Executor::execute()
{
	this->program->exec();
}

ast::DataType Executor::libAdapter(ast::FunctionCall* call)
{
	if (call->getIdentifier() == "print")
	{
		if (!checkArguments(call->getCallOperator(), { ast::DataType::String }))
			throw Error(Error::Type::IncorrectParametersList);
		call->returned = lib::print(std::get<std::string>(call->getCallOperator()->getArguments()[0]->returned));
		return ast::DataType::Int;
	}
}

bool Executor::checkArguments(ast::CallOperator* oper, std::initializer_list<ast::DataType> argTypes)
{
	if (oper->getArguments().size() != argTypes.size())
		return false;
	int i = 0;
	for (auto type : argTypes)
	{
		if (oper->getArguments()[i++]->getDataType() != type)
			return false;
		oper->getArguments()[0]->eval();
	}
	return true;
}

bool Executor::checkFunction(std::string identifier)
{
	for (auto i : lib::functions)
		if (i == identifier)
			return true;
	return false;
}

