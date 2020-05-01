#ifndef PROGRAM_H
#define PROGRAM_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class Program : public Node
		{
		public:
			void addFunction(std::shared_ptr<FunctionDef>& function)
			{
				function->parent = std::make_shared<Node>(*this);
				this->functions.push_back(function);
			}

			std::vector<std::shared_ptr<FunctionDef>>& getFunctions()
			{
				return functions;
			}

			std::string toString()
			{
				return "Program";
			}

		private:
			std::vector<std::shared_ptr<FunctionDef>> functions;
		};

	}
}

#endif // !PROGRAM_H

