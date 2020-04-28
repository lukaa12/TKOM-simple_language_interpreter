#ifndef AST_NODE_H
#define AST_NODE_H

#include <string>
#include <memory>
#include <vector>
#include <variant>
#include "../Token.h"

namespace tkom {
	namespace ast {

		class Node 
		{
		public:
			virtual std::string toString() = 0;
			virtual void eval() = 0;
		protected:
			std::weak_ptr<Node> parent;
		};

}
}


#include "AsignStatement.h"
#include "Body.h"
#include "CallDef.h"
#include "CallOperator.h"
#include "Conditions.h"
#include "Expressions.h"
#include "FunctionCall.h"
#include "FunctionDef.h"
#include "FunctionExec.h"
#include "IfStatement.h"
#include "InitStatement.h"
#include "Instruction.h"
#include "Program.h"
#include "ReturnStatement.h"
#include "RightValue.h"
#include "WhileLoop.h"

#endif // !AST_NODE_H
