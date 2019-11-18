#pragma once

#include "DrawUtils.hpp"
#include "Map.hpp"

namespace Pathfinder {
	class ConsoleRenderer {
		#define MAX_INPUT_SIZE 256

		public:
			ConsoleRenderer( MapLayer&& ml );
			~ConsoleRenderer();

			void operator()();


		private:
			void drawMapLayer();
			void handleUserInput();
			bool handleColon();

			MapLayer map_layer;
			int x_size, y_size;
			int x_offset, y_offset;
			int scale;
			bool running;
			char input[MAX_INPUT_SIZE];
			std::string filename;
	};
}
