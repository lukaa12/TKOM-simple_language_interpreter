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
		};

	}
}

#endif // !DATATYPES_H

