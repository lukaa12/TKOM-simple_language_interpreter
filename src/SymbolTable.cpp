#include "SymbolTable.h"
#include "stdlib/Functions.h"
#include "Error.h"

using namespace tkom;

SymbolTable::SymbolTable()
{
	for (auto i : lib::functions)
		this->addGlobalSymbol(Symbol(i.first, i.second));
}

Symbol* SymbolTable::getSymbol(std::string id)
{
	Symbol* symbol = nullptr;

	if (local.size() != 0)
		symbol = local.top().getSymbol(id);

	if (symbol == nullptr)
		symbol = this->global.getSymbol(id);

	if (symbol == nullptr)
	{
		throw Error("Undefined reference");
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
		throw Error("Cannot leave scope");
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
		throw Error("Symbol already definied in this scope");
}

Symbol::Symbol(ast::DataType _dtype, std::string _id): type(ast::IdType::Variable), dataType(_dtype), identifier(_id)
{}

Symbol::Symbol(ast::FunctionDef* ptr): type(ast::IdType::Function), dataType(ptr->getReturnType()),
identifier(ptr->getIdentifier()), value(ptr)
{}

Symbol::Symbol(): type(ast::IdType::Variable), dataType(ast::DataType::Int)
{}

Symbol::Symbol(std::string _id, ast::DataType t): type(ast::IdType::Function), dataType(t), identifier(_id)
{}
