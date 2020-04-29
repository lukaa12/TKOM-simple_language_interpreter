#ifndef AST_NODE_H
#define AST_NODE_H

#include <string>
#include <memory>
#include <vector>
#include <variant>
#include "../Token.h"
#include "../SymbolTable.h"

namespace tkom {
	namespace ast {

		class Node 
		{
		public:
			virtual std::string toString()
			{
				throw std::exception("TODO");
			}
			virtual void eval()
			{
				throw std::exception("TODO");
			}
		protected:
			std::weak_ptr<Node> parent;
		};

		class CallOperator;
		class FunctionCall;
		class RightValue;

}
}

#include "Expressions.h"
#include "CallOperator.h"
#include "FunctionCall.h"
#include "RightValue.h"
#include "Conditions.h"
#include "Instruction.h"
#include "RightValue.h"
#include "AsignStatement.h"
#include "Body.h"
#include "CallDef.h"
#include "FunctionDef.h"
#include "FunctionExec.h"
#include "IfStatement.h"
#include "InitStatement.h"
#include "ReturnStatement.h"
#include "WhileLoop.h"
#include "Program.h"

#endif // !AST_NODE_H
