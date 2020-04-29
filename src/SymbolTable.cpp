#include "SymbolTable.h"

using namespace tkom;

Symbol SymbolTable::getSymbol(std::string id)
{
	Symbol symbol;
	try
	{
		if (local.size() == 0)
			throw std::exception();
		symbol = local.top().getSymbol(id);
	}
	catch (const std::exception&)
	{
		try
		{
			symbol = this->global.getSymbol(id);
		}
		catch (const std::exception&)
		{
			throw std::exception("Symbol not found");
		}
		return symbol;
	}
	return symbol;
}

void SymbolTable::addGlobalSymbol(const Symbol& symbol)
{
	global.addSymbol(symbol);
}

void SymbolTable::addLocalSymbol(const Symbol& symbol)
{
	local.top().addSymbol(symbol);
}

void SymbolTable::enterScope()
{
	local.push(Scope());
}

void SymbolTable::leaveScope()
{
	if (local.size() == 0)
		throw std::exception("Cannot leave scope");
	local.pop();
}

Symbol Scope::getSymbol(std::string id)
{
	auto symbol = this->symbols.find(id);
	if (symbol == symbols.end())
		throw std::exception("Symbol not found");
	return symbol->second;
}

void Scope::addSymbol(const Symbol& symbol)
{
	if (!(this->symbols.insert({ symbol.identifier, symbol }).second))
		throw std::exception("Symbol already definied in this scope");
}

Symbol::Symbol(ast::IdType _type, ast::DataType _dtype, std::string _id): type(_type), dataType(_dtype), identifier(_id)
{}

//Symbol::Symbol(std::shared_ptr<ast::FunctionDef> ptr): type(Type::Function), dataType(ptr->returnType),
//identifier(ptr->identifier)
//{
//	value = ptr;
//}

