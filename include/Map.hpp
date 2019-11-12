#pragma once

#include <string>
#include <array>

namespace Pathfinder {
	struct MapCell {
		//TODO
	};
	struct MapLayer {
		public:
			MapLayer( const std::string& filename );
			~MapLayer();

			MapCell& operator[]( const std::array<size_t, 2>& xy );

			MapCell& at( size_t x, size_t y );

		private:
			size_t x_size, y_size;
			MapCell* cells;
	};

	struct Map {
		public:
			Map( const std::string& filename );
	};
}
