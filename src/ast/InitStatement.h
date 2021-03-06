#ifndef INIT_STATEMENT_H
#define INIT_STATEMENT_H

#include "AstNode.h"

namespace tkom {
	namespace ast {

		class InitStatement : public Instruction
		{
		public:
			InitStatement() = default;

			void setDataType(const DataType& type)
			{
				this->dataType = type;
			}

			DataType getDataType()
			{
				return dataType;
			}

			void addInitiated(std::pair<std::string, std::unique_ptr<RightValue>> pair)
			{
				if (pair.second != nullptr)
					pair.second->parent = this;
				this->initiated.push_back(std::move(pair));
			}

			std::vector<std::pair<std::string, std::unique_ptr<RightValue>>>& getInitiated()
			{
				return initiated;
			}

			Type getType()
			{
				return Type::Init;
			}

			void exec();

		private:
			DataType dataType;
			std::vector<std::pair<std::string, std::unique_ptr<RightValue>>> initiated;
		};

	}
}

#endif // !INIT_STATEMENT_H