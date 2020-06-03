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
			throw Error(call->pos, Error::Type::IncorrectParametersList);
		call->returned = lib::print(std::get<std::string>(call->getCallOperator()->getArguments()[0]->returned));
		return ast::DataType::Int;
	}
	else if (call->getIdentifier() == "show")
	{
		if (!checkArguments(call->getCallOperator(), { ast::DataType::Graphic }))
			throw Error(call->pos, Error::Type::IncorrectParametersList);
		call->returned = lib::show(std::get<lib::Graphic>(call->getCallOperator()->getArguments()[0]->returned));
		return ast::DataType::Int;
	}
	else if (call->getIdentifier() == "blank")
	{
		if (!checkArguments(call->getCallOperator(), { ast::DataType::Int, ast::DataType::Int }))
			throw Error(call->pos, Error::Type::IncorrectParametersList);
		call->returned = lib::blank(std::get<int>(call->getCallOperator()->getArguments()[0]->returned),
			std::get<int>(call->getCallOperator()->getArguments()[1]->returned));
		return ast::DataType::Graphic;
	}
	else if (call->getIdentifier() == "triangle")
	{
		if (!checkArguments(call->getCallOperator(), { ast::DataType::Int, ast::DataType::Int }))
			throw Error(call->pos, Error::Type::IncorrectParametersList);
		call->returned = lib::triangle(std::get<int>(call->getCallOperator()->getArguments()[0]->returned),
			std::get<int>(call->getCallOperator()->getArguments()[1]->returned));
		return ast::DataType::Graphic;
	}
	else if (call->getIdentifier() == "rectangular_triangle")
	{
		if (!checkArguments(call->getCallOperator(), { ast::DataType::Int, ast::DataType::Int }))
			throw Error(call->pos, Error::Type::IncorrectParametersList);
		call->returned = lib::rectangularTriangle(std::get<int>(call->getCallOperator()->getArguments()[0]->returned),
			std::get<int>(call->getCallOperator()->getArguments()[1]->returned));
		return ast::DataType::Graphic;
	}
	else if (call->getIdentifier() == "rectangle")
	{
		if (!checkArguments(call->getCallOperator(), { ast::DataType::Int, ast::DataType::Int }))
			throw Error(call->pos, Error::Type::IncorrectParametersList);
		call->returned = lib::rectangle(std::get<int>(call->getCallOperator()->getArguments()[0]->returned),
			std::get<int>(call->getCallOperator()->getArguments()[1]->returned));
		return ast::DataType::Graphic;
	}
	else if (call->getIdentifier() == "circle")
	{
		if (!checkArguments(call->getCallOperator(), { ast::DataType::Int }))
			throw Error(call->pos, Error::Type::IncorrectParametersList);
		call->returned = lib::circle(std::get<int>(call->getCallOperator()->getArguments()[0]->returned));
		return ast::DataType::Graphic;
	}
	else if (call->getIdentifier() == "line")
	{
		if (!checkArguments(call->getCallOperator(), { ast::DataType::Int }))
			throw Error(call->pos, Error::Type::IncorrectParametersList);
		call->returned = lib::line(std::get<int>(call->getCallOperator()->getArguments()[0]->returned));
		return ast::DataType::Graphic;
	}
	else if (call->getIdentifier() == "add")
	{
		if (!checkArguments(call->getCallOperator(), { ast::DataType::Graphic, ast::DataType::Graphic }))
			throw Error(call->pos, Error::Type::IncorrectParametersList);
		call->returned = lib::add(std::get<lib::Graphic>(call->getCallOperator()->getArguments()[0]->returned),
			std::get<lib::Graphic>(call->getCallOperator()->getArguments()[1]->returned));
		return ast::DataType::Graphic;
	}
	else if (call->getIdentifier() == "translate")
	{
		if (!checkArguments(call->getCallOperator(), { ast::DataType::Graphic, ast::DataType::Int, ast::DataType::Int }))
			throw Error(call->pos, Error::Type::IncorrectParametersList);
		call->returned = lib::translate(std::get<lib::Graphic>(call->getCallOperator()->getArguments()[0]->returned),
			std::get<int>(call->getCallOperator()->getArguments()[1]->returned),
			std::get<int>(call->getCallOperator()->getArguments()[2]->returned));
		return ast::DataType::Graphic;
	}
	else if (call->getIdentifier() == "scale")
	{
		if (!checkArguments(call->getCallOperator(), { ast::DataType::Graphic, ast::DataType::Int, ast::DataType::Int }))
			throw Error(call->pos, Error::Type::IncorrectParametersList);
		call->returned = lib::scale(std::get<lib::Graphic>(call->getCallOperator()->getArguments()[0]->returned),
			std::get<int>(call->getCallOperator()->getArguments()[1]->returned),
			std::get<int>(call->getCallOperator()->getArguments()[2]->returned));
		return ast::DataType::Graphic;
	}
	else if (call->getIdentifier() == "rotate")
	{
		if (!checkArguments(call->getCallOperator(), { ast::DataType::Graphic, ast::DataType::Int }))
			throw Error(call->pos, Error::Type::IncorrectParametersList);
		call->returned = lib::rotate(std::get<lib::Graphic>(call->getCallOperator()->getArguments()[0]->returned),
			std::get<int>(call->getCallOperator()->getArguments()[1]->returned));
		return ast::DataType::Graphic;
	}
	else if (call->getIdentifier() == "unfill")
	{
		if (!checkArguments(call->getCallOperator(), { ast::DataType::Graphic }))
			throw Error(call->pos, Error::Type::IncorrectParametersList);
		call->returned = lib::unFill(std::get<lib::Graphic>(call->getCallOperator()->getArguments()[0]->returned));
		return ast::DataType::Graphic;
	}
	else if (call->getIdentifier() == "color_rgb")
	{
		if (!checkArguments(call->getCallOperator(), { ast::DataType::Int, ast::DataType::Int, ast::DataType::Int }))
			throw Error(call->pos, Error::Type::IncorrectParametersList);
		call->returned = lib::colorRGB(std::get<int>(call->getCallOperator()->getArguments()[0]->returned),
		std::get<int>(call->getCallOperator()->getArguments()[1]->returned),
		std::get<int>(call->getCallOperator()->getArguments()[2]->returned));
		return ast::DataType::Color;
	}
	else if (call->getIdentifier() == "set_color")
	{
	if (!checkArguments(call->getCallOperator(), { ast::DataType::Graphic, ast::DataType::Color }))
		throw Error(call->pos, Error::Type::IncorrectParametersList);
	call->returned = lib::setColor(std::get<lib::Graphic>(call->getCallOperator()->getArguments()[0]->returned),
		std::get<lib::Color>(call->getCallOperator()->getArguments()[1]->returned));
	return ast::DataType::Graphic;
	}
	throw Error();
}

bool Executor::checkArguments(ast::CallOperator* oper, std::initializer_list<ast::DataType> argTypes)
{
	if (oper->getArguments().size() != argTypes.size())
		return false;
	int i = 0;
	for (auto type : argTypes)
	{
		if (oper->getArguments()[i]->getDataType() != type)
			return false;
		oper->getArguments()[i++]->eval();
	}
	return true;
}

bool Executor::checkFunction(std::string identifier)
{
	for (auto i : lib::functions)
		if (i.first == identifier)
			return true;
	return false;
}

