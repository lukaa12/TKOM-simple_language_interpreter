#include "AstNode.h"

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

void BracketExpression::setExpression(const std::shared_ptr<Expression>& ptr)
{
	ptr->parent = std::make_shared<Node>(*this);
	expression = ptr;
}

void BracesCondition::setCondition(const std::shared_ptr<Condition>& ptr)
{
	ptr->parent = std::make_shared<Node>(*this);
	condition = ptr;
}
