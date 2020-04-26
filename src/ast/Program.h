#ifndef PROGRAM_H
#define PROGRAM_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class Program : public Node
		{
		public:
			void addFunction(const std::shared_ptr<FunctionDef>& function)
			{
				this->functions.push_back(function);
			}

			std::string toString()
			{
				return "Program";
			}

			std::vector<std::shared_ptr<FunctionDef>> functions = {};
		};

	}
}

#endif // !PROGRAM_H

