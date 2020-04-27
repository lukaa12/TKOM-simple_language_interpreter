#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <ostream>
#include <unordered_map>
#include <stack>

namespace tkom {

	class Symbol
	{
	public:
		enum class Type {
			Function,
			Variable
		} type;

		enum class DataType {
			Int,
			String,
			Graphic,
			Color
		} dataType;

		std::string identifier;
		int value; //will be replaced by std::variant

		Symbol(Type _type, DataType _dtype, std::string _id);
		Symbol() = default;

		static std::string toString(Type type);
		static std::string toString(DataType type);
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
	private:
		Scope global;
		std::stack<Scope> local;
	};

	inline std::ostream& operator<<(std::ostream& o, Symbol::Type type)
	{
		return o << Symbol::toString(type);
	}

	inline std::ostream& operator<<(std::ostream& o, Symbol::DataType type)
	{
		return o << Symbol::toString(type);
	}

}

#endif // !SYMBOL_TABLE_H
