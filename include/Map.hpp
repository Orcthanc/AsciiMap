#pragma once

#include <string>
#include <array>
#include <vector>

namespace Pathfinder {

	enum class WallType: uint16_t {
		none = 0,
		wall = 1,
		door = 2,
		window = 3,
		secret_door = 4,
	};

	enum class FloorType: uint16_t {
		normal = 0,
		trapdoor = 1,
		secret_trap_door = 2,
		pressure_plate = 3,
		burning = 4,
	};

	struct Walls {
		//south and east are given by the tiles to the south and east respectively
		std::underlying_type_t<WallType> north, west;
		std::underlying_type_t<FloorType> current;
	};

	struct MapCell {
		friend std::istream& operator>>( std::istream&, MapCell& );
		friend std::ostream& operator<<( std::ostream&, const MapCell& );
		//TODO
		uint16_t area_code;
		Walls walls;
	};

	struct MapLayer {
		public:
			MapLayer() = default;

			[[deprecated("For testing purposes, do not use")]]
			MapLayer( const std::vector<MapCell>& cells, uint32_t x_size, uint32_t y_size );
			~MapLayer();

			MapLayer( const MapLayer& ) = delete;
			MapLayer( MapLayer&& );


			friend std::istream& operator>>( std::istream&, MapLayer& );
			friend std::ostream& operator<<( std::ostream&, const MapLayer& );

			MapLayer& operator=( const MapLayer& ) = delete;
			MapLayer& operator=( MapLayer&& );

			const MapCell& operator[]( const std::array<uint32_t, 2>& xy ) const noexcept;
			MapCell& operator[]( const std::array<uint32_t, 2>& xy ) noexcept;


			const MapCell& at( uint32_t x, uint32_t y ) const noexcept;
			MapCell& at( uint32_t x, uint32_t y ) noexcept;



			uint32_t x_size, y_size;
		private:
			MapCell* cells;
	};

	struct Map {
		public:
			Map( const std::string& filename );
	};



	std::istream& operator>>( std::istream&, MapCell& );
	std::ostream& operator<<( std::ostream&, const MapCell& );

	std::istream& operator>>( std::istream&, MapLayer& );
	std::ostream& operator<<( std::ostream&, const MapLayer& );
}
