#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>

namespace tkom {

	class Symbol;
	class Scope;

	class SymbolTable
	{

	};

	class Symbol {
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
	};

}

#endif // !SYMBOL_TABLE_H
