#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <stack>
#include <variant>
#include "ast/AstNode.h"

namespace tkom {

	class Symbol
	{
	public:
		ast::IdType type;
		ast::DataType dataType;
		std::string identifier;
		std::variant<int, std::string, ast::FunctionDef*> value;

		Symbol(ast::DataType _dtype, std::string _id);
		Symbol(ast::FunctionDef* ptr);
		Symbol();
	};

	class Scope
	{
	public:
		Symbol getSymbol(std::string id);
		void addSymbol(const Symbol& symbol);
	private:
		std::unordered_map<std::string, Symbol> symbols;
	};

	class SymbolTable
	{
	public:
		Symbol getSymbol(std::string id);
		void addGlobalSymbol(const Symbol& symbol);
		void addLocalSymbol(const Symbol& symbol);
		void enterScope();
		void leaveScope();
		void leaveAllScopes();
	private:
		Scope global;
		std::stack<Scope> local;
	};


}

#endif // !SYMBOL_TABLE_H
