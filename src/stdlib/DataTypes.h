#ifndef DATATYPES_H
#define DATATYPES_H

namespace tkom {
	namespace lib {

		class Color
		{
		public:
			short r;
			short g;
			short b;
		};

		class Operation
		{
		public:
			enum class Type {
				Pane,
				Triangle,
				Circle,
				Rectangle,
				Line,
				Scale,
				Rotate,
				Move,
				AddObject,
			};
		};

		class Graphic
		{
		public:
			Color color;
			struct Position
			{
				int x;
				int y;
				int rotation;
				int percentScaleX;
				int percentScaleY;
			} position;
			//std::vector
		};

		

	}
}

#endif // !DATATYPES_H

