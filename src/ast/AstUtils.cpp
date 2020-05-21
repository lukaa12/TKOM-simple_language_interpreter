#include "AstNode.h"
#include "../Executor.h"
#include "../Error.h"

using namespace tkom;
using namespace ast;

std::string tkom::ast::toString(const RelationOperator& op)
{
	switch (op)
	{
	case RelationOperator::Less:
		return "<";
	case RelationOperator::LessEqual:
		return "<=";
	case RelationOperator::Equal:
		return "==";
	case RelationOperator::GreaterEqual:
		return ">=";
	case RelationOperator::Greater:
		return ">";
	case RelationOperator::NotEqual:
		return "!=";
	}
	return "Unknown Relation Operator";
}

std::string tkom::ast::toString(const IdType& type)
{
	switch (type)
	{
	case IdType::Function:
		return "Function";
	case IdType::Variable:
		return "Variable";
	}
	return "Unknown";
}

std::string tkom::ast::toString(const DataType& type)
{
	switch (type)
	{
	case DataType::Color:
		return "Color";
	case DataType::Graphic:
		return "Graphic";
	case DataType::Int:
		return "Int";
	case DataType::String:
		return "String";
	}
	return "Unknown";
}

void BracketExpression::setExpression(std::unique_ptr<Expression> ptr)
{
	ptr->parent = this;
	expression = std::move(ptr);
}

void BracesCondition::setCondition(std::unique_ptr<Condition> ptr)
{
	ptr->parent = this;
	condition = std::move(ptr);
}

void Program::exec()
{
	for(auto it = functions.begin(); it != functions.end(); ++it)
		if (it->get()->getIdentifier() == "main")
		{
			Executor::symbolTable.enterScope();
			it->get()->getFunctionBody()->exec();
			return;
		}
	throw Error(Error::Type::MissingMain);
}

void PrimaryExpression::exec()
{
	throw std::exception("TODO");
	//switch (type)
	//{
	//case PrimaryExpression::Type::Literal:
	//	evaluated = std::get<int>(value);
	//	break;
	//case PrimaryExpression::Type::Identifier:
	//	Symbol* symbol = &(Executor::symbolTable.getSymbol(std::get<std::string>(value)));
	//	if (symbol->dataType != DataType::Int)
	//		throw Error(Error::Type::UncompatibleType);
	//	evaluated = std::get<int>(symbol->value);
	//	break;
	//case PrimaryExpression::Type::Function:
	//	FunctionCall* func = this->getValue<FunctionCall>();
	//	if(func->getIdentifier())
	//	break;
	//case PrimaryExpression::Type::Bracket:
	//	break;
	//default:
	//	break;
	//}
}

int PrimaryExpression::eval()
{
	this->exec();
	throw std::exception("TODO");
}

void FunctionCall::exec()
{
	Symbol* myFunction = nullptr;
	try
	{
		myFunction = &Executor::symbolTable.getSymbol(identifier);
	}
	catch (const std::exception& e)
	{
		if (e.what() == "Undefined reference")
			throw Error(Error::Type::UndefinedReference);
	}
	if (myFunction->type != IdType::Function)
		throw Error(Error::Type::CallOnNonFunction);
	if(!checkArguments())
		throw Error(Error::Type::IncorrectParametersList);
	assignArguments();
	FunctionDef* def = std::get<FunctionDef*>(myFunction->value);
	def->getFunctionBody()->exec();
}

int FunctionCall::returnInt()
{
	throw std::exception("TODO");
}

std::string FunctionCall::returnString()
{
	throw std::exception("TODO");
}

bool FunctionCall::checkArguments()
{
	CallDef* def = std::get<FunctionDef*>(Executor::symbolTable.getSymbol(identifier).value)->getCallDef();
	if (def->getArgumenst().size() != callOperator->getArguments().size())
		return false;
	for(auto i = 0; i != def->getArgumenst().size(); ++i)
		if(def->getArgumenst()[i].first != callOperator->getArguments()[i]->getDataType())
			return false;
			
	return true;
}

void FunctionCall::assignArguments()
{
	Executor::symbolTable.enterScope();
	CallDef* def = std::get<FunctionDef*>(Executor::symbolTable.getSymbol(identifier).value)->getCallDef();
	for (auto i = 0; i != def->getArgumenst().size(); ++i)
	{
		Symbol sym = Symbol(def->getArgumenst()[i].first, def->getArgumenst()[i].second);
		switch (def->getArgumenst()[i].first)
		{
		case DataType::Int:
			sym.value = callOperator->getArguments()[i]->intValue();
			break;
		case DataType::String:
			sym.value = callOperator->getArguments()[i]->stringValue();
			break;
		}
		Executor::symbolTable.addLocalSymbol(sym);
	}
}

DataType RightValue::getDataType()
{
	if (type == Type::Expression || type == Type::IntLiteral)
		return DataType::Int;
	if (type == Type::StringLiteral)
		return DataType::String;
	else
	{
		Symbol* sym = nullptr;
		try
		{
			sym = &(Executor::symbolTable.getSymbol(std::get<std::string>(value)));
		}
		catch (const std::exception& e)
		{
			if (e.what() == "Undefined reference")
				throw Error(Error::Type::UndefinedReference);
		}
		return sym->dataType;
	}
}

void RightValue::exec()
{
	throw std::exception("TODO");
}

int RightValue::intValue()
{
	exec();
	throw std::exception("TODO");
}

std::string RightValue::stringValue()
{
	exec();
	throw std::exception("TODO");
}
