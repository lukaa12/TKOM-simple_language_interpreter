#ifndef BODY_H
#define BODY_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class Body : public Node
		{
		public:
			std::vector<std::unique_ptr<Instruction>> instructions;
		};

	}
}

#endif // !BODY_H