#pragma once

#include "DrawUtils.hpp"
#include "Map.hpp"

namespace Pathfinder {
	class ConsoleRenderer {
		public:
			ConsoleRenderer( MapLayer&& ml );
			~ConsoleRenderer();

			void operator()();


		private:
			void drawMapLayer();
			void handleUserInput();

			MapLayer map_layer;
			int x_offset, y_offset;
			bool running;
	};
}
