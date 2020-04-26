#ifndef FUNCTION_DEF_H
#define FUNCTION_DEF_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class FunctionDef : public Node
		{
		public:
			Token::Type returnType;
			
		};

	}
}

#endif // !FUNCTION_DEF_H