#include "AstNode.h"
#include "../Executor.h"
#include "../Error.h"
#include <iostream>

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

//AST EXECUTION METHODS

int Program::exec()
{
	for(auto it = functions.begin(); it != functions.end(); ++it)
		if (it->get()->getIdentifier() == "main")
		{
			Executor::symbolTable.enterScope();
			if (it->get()->getFunctionBody()->exec() != DataType::Int)
				throw Error(Error::Type::UncompatibleType);
			if (!(it->get()->getFunctionBody()->wasReturn))
				throw Error(Error::Type::FunctionNotReturnedValue);
			return std::get<int>(it->get()->getFunctionBody()->returned);
		}
	throw Error(Error::Type::MissingMain);
}

//EXPRESSIONS

int Expression::eval()
{
	int value = 0;
	for (auto it = components.begin(); it != components.end(); ++it)
	{
		if (it->first)
			value -= it->second->eval();
		else
			value += it->second->eval();
	}
	return value;
}

int MultiplicativeExpression::eval()
{
	int value = components[0]->eval();
	for (int i = 0; i < divisionFlags.size(); ++i)
	{
		if (!divisionFlags[i])
			value *= components[i + 1]->eval();
		else
		{
			int factor = components[i + 1]->eval();
			if (factor == 0)
				throw Error(Error::Type::DivisionByZero);
			value /= factor;
		}
	}
	return value;
}

int PrimaryExpression::eval()
{
	Symbol* symbol = nullptr;
	FunctionCall* func = nullptr;
	switch (type)
	{
	case PrimaryExpression::Type::Literal:
		return std::get<int>(value);
	case PrimaryExpression::Type::Identifier:
		symbol = Executor::symbolTable.getSymbol(std::get<std::string>(value));
		if (symbol->dataType != DataType::Int)
			throw Error(Error::Type::UncompatibleType);
		return std::get<int>(symbol->value);
	case PrimaryExpression::Type::Function:
		func = this->getValue<FunctionCall>();
		if (func->exec() != DataType::Int)
			throw Error(Error::Type::UncompatibleType);
		return std::get<int>(func->returned);
	case PrimaryExpression::Type::Bracket:
		return this->getValue<BracketExpression>()->getExpression()->eval();
	}
}

//CONDITIONS

int Condition::eval()
{
	int val = 0;
	for (auto it = getComponents().begin(); it != getComponents().end(); ++it)
	{
		val += it->get()->eval() == 0 ? 0 : 1;
		val = val == 0 ? 0 : 1;
	}
	return val;
}

int AndCondition::eval()
{
	int val = 1;
	for (auto it = getComponents().begin(); it != getComponents().end(); ++it)
		val *= it->get()->eval();

	return val;
}

int RelationCondition::eval()
{
	if (getSecond() == nullptr)
		return getFirst()->eval() == 0 ? 0 : 1;
	else{
		switch (relationOper)
		{
		case RelationOperator::Equal:
			return getFirst()->eval() == getSecond()->eval() ? 1 : 0;
		case RelationOperator::Greater:
			return getFirst()->eval() > getSecond()->eval() ? 1 : 0;
		case RelationOperator::GreaterEqual:
			return getFirst()->eval() >= getSecond()->eval() ? 1 : 0;
		case RelationOperator::Less:
			return getFirst()->eval() < getSecond()->eval() ? 1 : 0;
		case RelationOperator::LessEqual:
			return getFirst()->eval() >= getSecond()->eval() ? 1 : 0;
		case RelationOperator::NotEqual:
			return getFirst()->eval() != getSecond()->eval() ? 1 : 0;
		}
	}
}

int PrimaryCondition::eval()
{
	int eval;
	if (type == Type::Bracket)
	{
		eval = getCondition<BracesCondition>()->getCondition()->eval();
		if (negated)
			return eval == 0 ? 1 : 0;
		else
			return eval;
	}
	else if (type == Type::RightVal)
	{
		switch (getCondition<RightValue>()->eval())
		{
		case DataType::Int:
			eval = std::get<int>(getCondition<RightValue>()->returned);
			break;
		default:
			throw Error(Error::Type::UncompatibleType);
			break;
		}
		if (negated)
			return eval == 0 ? 1 : 0;
		else
			return eval;
	}
}

//FUNCTION CALLING

DataType FunctionCall::exec()
{
	Symbol* myFunction = nullptr;
	try
	{ myFunction = Executor::symbolTable.getSymbol(identifier); }
	catch (const std::exception& e)
	{ if (e.what() == "Undefined reference")
			throw Error(Error::Type::UndefinedReference); }

	if (myFunction->type != IdType::Function)
		throw Error(Error::Type::CallOnNonFunction);
	if(!checkArguments())
		throw Error(Error::Type::IncorrectParametersList);
	
	Executor::symbolTable.enterScope();
	assignArguments();
	FunctionDef* def = std::get<FunctionDef*>(myFunction->value);
	
	if (def->getReturnType() != def->getFunctionBody()->exec())
		throw Error(Error::Type::UncompatibleType);
	if (!(def->getFunctionBody()->wasReturn))
		throw Error(Error::Type::FunctionNotReturnedValue);
	
	returned = def->getFunctionBody()->returned;
	Executor::symbolTable.leaveScope();
	return def->getReturnType();
}

bool FunctionCall::checkArguments()
{
	CallDef* def = std::get<FunctionDef*>(Executor::symbolTable.getSymbol(identifier)->value)->getCallDef();
	if (def->getArgumenst().size() != callOperator->getArguments().size())
		return false;
	for(auto i = 0; i != def->getArgumenst().size(); ++i)
		if(def->getArgumenst()[i].first != callOperator->getArguments()[i]->getDataType())
			return false;
			
	return true;
}

void FunctionCall::assignArguments()
{
	CallDef* def = std::get<FunctionDef*>(Executor::symbolTable.getSymbol(identifier)->value)->getCallDef();
	for (auto i = 0; i != def->getArgumenst().size(); ++i)
	{
		Symbol sym = Symbol(def->getArgumenst()[i].first, def->getArgumenst()[i].second);
		callOperator->getArguments()[i]->eval();
		switch (def->getArgumenst()[i].first)
		{
		case DataType::Int:
			sym.value = std::get<int>(callOperator->getArguments()[i]->returned);
			break;
		case DataType::String:
			sym.value = std::get<std::string>(callOperator->getArguments()[i]->returned);
			break;
		}
		Executor::symbolTable.addLocalSymbol(sym);
	}
}

DataType Body::exec()
{
	wasBreak = false;
	wasReturn = false;
	for (auto it = instructions.begin(); it != instructions.end(); ++it)
	{
		(*it)->exec();
		if ((*it)->getType() == Instruction::Type::Break)
		{
			auto parent = (*it)->parent;
			while (parent->toString() != "WhileLoop")
			{
				if (parent->parent == nullptr)
					throw Error(Error::Type::BreakOutsideLoop);
				parent = parent->parent;
			}
			wasBreak = true;
			return DataType::Int;
		}
		else if ((*it)->getType() == Instruction::Type::Return)
		{
			ReturnStatement* instr = dynamic_cast<ReturnStatement*>(it->get());
			this->returned = instr->returned;
			wasReturn = true;
			return instr->dtype;
		}
		else if ((*it)->getType() == Instruction::Type::While)
		{
			WhileLoop* loop = dynamic_cast<WhileLoop*>(it->get());
			if (loop->wasReturned)
			{
				wasReturn = true;
				this->returned = loop->returned;
				return loop->dtype;
			}
		}
		else if ((*it)->getType() == Instruction::Type::If)
		{
			auto ifStat = dynamic_cast<IfStatement*>(it->get());
			if (ifStat->wasReturned)
			{
				wasReturn = true;
				this->returned = ifStat->returned;
				return ifStat->dtype;
			}
			wasBreak = wasBreak ? true : ifStat->wasBreaked;
			return DataType::Int;
		}
	}
	return DataType::Int;
}

//INSTRUCTIONS EXECS

void Instruction::exec()
{
	//BRAKE STATEMENT NOTHING TO DO
}

void ReturnStatement::exec()
{
	this->dtype = this->value->eval();
	this->returned = this->value->returned;
}

void WhileLoop::exec()
{
	throw std::exception("TODO");
}

void IfStatement::exec()
{
	throw std::exception("TODO");
}

void FunctionExec::exec()
{

}

void InitStatement::exec()
{
	for (auto it = initiated.begin(); it != initiated.end(); ++it)
	{
		Symbol newSymbol{ dataType, it->first };
		if (it->second == nullptr)
		{
			Executor::symbolTable.addLocalSymbol(newSymbol);
			continue;
		}
		if (it->second->eval() != dataType)
			throw Error(Error::Type::UncompatibleType);
		switch (dataType)
		{
		case DataType::Int:
			newSymbol.value = std::get<int>(it->second->returned);
			break;
		case DataType::String:
			newSymbol.value = std::get<std::string>(it->second->returned);
			break;
		}
		Executor::symbolTable.addLocalSymbol(newSymbol);
	}
}

void AssignStatement::exec()
{
	Symbol* lval = nullptr;
	try
	{ lval = Executor::symbolTable.getSymbol(identifier); }
	catch (const std::exception& e)
	{ 
		std::cout << e.what() << std::endl;
		if (e.what() == "Undefined reference")
			throw Error(Error::Type::UndefinedReference); 
	}
	if (rvalue->eval() != lval->dataType)
		throw Error(Error::Type::UncompatibleType);
	switch (lval->dataType)
	{
	case DataType::Int:
		lval->value = std::get<int>(rvalue->returned);
		break;
	case DataType::String:
		lval->value = std::get<std::string>(rvalue->returned);
		break;
	}
}

//RIGHT VALUES EVALUATION

DataType RightValue::getDataType()
{
	switch (type)
	{
	case Type::Expression:
		return DataType::Int;
	case Type::IntLiteral:
		return DataType::Int;
	case Type::StringLiteral:
		return DataType::String;
	default:
	{
		Symbol* sym = nullptr;
		try
		{ sym = Executor::symbolTable.getSymbol(std::get<std::string>(value)); }
		catch (const std::exception & e)
		{ if (e.what() == "Undefined reference")
				throw Error(Error::Type::UndefinedReference); }
		return sym->dataType;
	}
	}
}

DataType RightValue::eval()
{
	switch (type)
	{
	case Type::Expression:
		returned = std::get<std::unique_ptr<Expression>>(value)->eval();
		return DataType::Int;
	case Type::IntLiteral:
		returned = std::get<int>(value);
		return DataType::Int;
	case Type::StringLiteral:
		returned = std::get<std::string>(value);
		return DataType::String;
	case Type::Identifier:
	{
		Symbol *sym = nullptr;
		try
		{ sym = Executor::symbolTable.getSymbol(std::get<std::string>(value)); }
		catch (const std::exception & e)
		{ if (e.what() == "Undefined reference")
				throw Error(Error::Type::UndefinedReference); }
		switch (sym->dataType)
		{
		case DataType::Int:
			returned = std::get<int>(sym->value);
			break;
		case DataType::String:
			returned = std::get<std::string>(sym->value);
			break;
		}
		return sym->dataType;
	}
	case Type::Function:
	{
		DataType type = getValue<FunctionCall>()->exec();
		returned = getValue<FunctionCall>()->returned;
		return type;
	}
	}
}
