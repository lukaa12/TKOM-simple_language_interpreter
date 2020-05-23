#include "SymbolTable.h"

using namespace tkom;

Symbol* SymbolTable::getSymbol(std::string id)
{
	Symbol* symbol = nullptr;

	if (local.size() != 0)
		symbol = local.top().getSymbol(id);

	if (symbol == nullptr)
		symbol = this->global.getSymbol(id);

	if (symbol == nullptr)
		throw std::exception("Undefined reference to");
	
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

void SymbolTable::leaveAllScopes()
{
	while (local.size() != 0)
		local.pop();
}

Symbol* Scope::getSymbol(std::string id)
{
	auto symbol = this->symbols.find(id);
	if (symbol == symbols.end())
		return nullptr;
	return &symbol->second;
}

void Scope::addSymbol(const Symbol& symbol)
{
	if (!(this->symbols.insert({ symbol.identifier, symbol }).second))
		throw std::exception("Symbol already definied in this scope");
}

Symbol::Symbol(ast::DataType _dtype, std::string _id): type(ast::IdType::Variable), dataType(_dtype), identifier(_id)
{}

Symbol::Symbol(ast::FunctionDef* ptr): type(ast::IdType::Function), dataType(ptr->getReturnType()),
identifier(ptr->getIdentifier()), value(ptr)
{}

Symbol::Symbol(): type(ast::IdType::Variable), dataType(ast::DataType::Int)
{}