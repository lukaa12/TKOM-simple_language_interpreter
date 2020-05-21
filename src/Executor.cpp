#include "Executor.h"

using namespace tkom;

SymbolTable Executor::symbolTable = SymbolTable();

Executor::Executor(ast::Program* prog) : program(prog)
{

}

void Executor::execute()
{
	this->program->eval();
}