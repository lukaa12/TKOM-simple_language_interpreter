#ifndef AST_NODE_H
#define AST_NODE_H

#include <string>
#include <memory>
#include <vector>
#include <variant>
#include <ostream>

namespace tkom {
	namespace ast {

		enum class DataType {
			Int,
			String,
			Graphic,
			Color
		};

		enum class IdType {
			Function,
			Variable
		};

		enum class RelationOperator {
			Less,
			LessEqual,
			Equal,
			GreaterEqual,
			Greater,
			NotEqual
		};

		std::string toString(const IdType& type);
		std::string toString(const DataType& type);
		std::string toString(const RelationOperator& op);

		class Node 
		{
		public:
			Node() = default;

			virtual std::string toString()
			{
				throw std::exception("TODO");
			}
			virtual void eval()
			{
				throw std::exception("TODO");
			}
		
			std::weak_ptr<Node> parent;
		};

		class CallOperator;
		class FunctionCall;
		class RightValue;

		inline std::ostream& operator<<(std::ostream& o, const ast::IdType type)
		{
			return o << toString(type);
		}

		inline std::ostream& operator<<(std::ostream& o, const ast::DataType type)
		{
			return o << toString(type);
		}

		inline std::ostream& operator<<(std::ostream& o, const ast::RelationOperator oper)
		{
			return o << toString(oper);
		}

}
}

#include "Expressions.h"
#include "CallOperator.h"
#include "FunctionCall.h"
#include "RightValue.h"
#include "Conditions.h"
#include "Instruction.h"
#include "RightValue.h"
#include "AssignStatement.h"
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
