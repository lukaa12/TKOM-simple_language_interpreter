#include "AstNode.h"
#include "../Executor.h"
#include "../Error.h"

using namespace tkom::ast;

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

void Program::eval()
{
	for(auto it = functions.begin(); it != functions.end(); ++it)
		if (it->get()->getIdentifier() == "main")
		{
			Executor::symbolTable.enterScope();
			it->get()->eval();
			return;
		}
	throw Error(Error::Type::MissingMain);
}