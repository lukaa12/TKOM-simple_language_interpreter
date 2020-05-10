#ifndef PROGRAM_H
#define PROGRAM_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class Program : public Node
		{
		public:
			void addFunction(std::unique_ptr<FunctionDef> function)
			{
				function->parent = this;
				this->functions.push_back(std::move(function));
			}

			std::vector<std::unique_ptr<FunctionDef>>& getFunctions()
			{
				return functions;
			}

			std::string toString()
			{
				return "Program";
			}

		private:
			std::vector<std::unique_ptr<FunctionDef>> functions;
		};

	}
}

#endif // !PROGRAM_H

