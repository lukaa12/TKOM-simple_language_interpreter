#ifndef CALL_DEF_H
#define CALL_DEF_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class CallDef : public Node
		{
		public:
			std::vector<std::pair<DataType, std::string>>& getArgumenst()
			{
				return argumentsList;
			}

			void addArgument(std::pair<DataType, std::string> arg)
			{
				argumentsList.push_back(arg);
			}

		private:
			std::vector<std::pair<DataType, std::string>> argumentsList;
		};

	}
}

#endif // !CALL_DEF_H