#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <iostream>
#include <unordered_map>
#include "DataTypes.h"
#include "../ast/AstNode.h"

namespace tkom {
	namespace lib {

		int print(std::string in);

		int show(Graphic image);

		Graphic blank(int w, int h);

		Graphic triangle(int h, int w);

		Graphic rectangularTriangle(int a, int b);

		Graphic rectangle(int a, int b);

		Graphic circle(int r);

		Graphic line(int len);

		Graphic add(Graphic parent, Graphic child);

		Graphic translate(Graphic object, int x, int y);

		Graphic scale(Graphic object, int x, int y);

		Graphic rotate(Graphic object, int x);

		Graphic unFill(Graphic object);

		Color colorRGB(int r, int g, int b);

		Graphic setColor(Graphic obj, Color c);


		static const std::vector<std::pair<std::string, ast::DataType>> functions = {
			std::make_pair("print", ast::DataType::Int),
			std::make_pair("show", ast::DataType::Int),
			std::make_pair("blank", ast::DataType::Graphic),
			std::make_pair("triangle", ast::DataType::Graphic),
			std::make_pair("rectangular_triangle", ast::DataType::Graphic),
			std::make_pair("rectangle", ast::DataType::Graphic),
			std::make_pair("circle", ast::DataType::Graphic),
			std::make_pair("line", ast::DataType::Graphic),
			std::make_pair("add", ast::DataType::Graphic),
			std::make_pair("translate", ast::DataType::Graphic),
			std::make_pair("scale", ast::DataType::Graphic),
			std::make_pair("rotate", ast::DataType::Graphic),
			std::make_pair("unfill", ast::DataType::Graphic),
			std::make_pair("color_rgb", ast::DataType::Color),
			std::make_pair("set_color", ast::DataType::Graphic)
		};

	}
}
 
#endif // !FUNCTIONS_H
