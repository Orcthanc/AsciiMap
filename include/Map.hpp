#pragma once

#include <string>
#include <array>
#include <vector>
#include <fstream>

namespace Pathfinder {

	struct MapCell;
	struct MapLayer;

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

	struct BinSIStream {
		BinSIStream( std::istream& is ): is( is ){}

		BinSIStream& operator>>( MapCell& );
		BinSIStream& operator>>( MapLayer& );

		template<typename T>
		std::enable_if_t<std::is_integral_v<T>, BinSIStream&> operator>>( T& t ){
			//TODO little/big endian
			is.read(( char* )&t, sizeof( T ));
			return *this;
		}

		std::istream& is;
	};

	struct BinSOStream {
		BinSOStream( std::ostream& os ): os( os ){}

		BinSOStream& operator<<( const MapCell& );
		BinSOStream& operator<<( const MapLayer& );

		template<typename T>
		std::enable_if_t<std::is_integral_v<T>, BinSOStream&> operator<<( const T& t ){
			os.write(( char* )&t, sizeof( T ));
			return *this;
		}

		std::ostream& os;
	};

	struct MapCell {
		friend std::istream& operator>>( std::istream&, MapCell& );
		friend std::ostream& operator<<( std::ostream&, const MapCell& );

		friend BinSIStream& BinSIStream::operator>>( MapCell& );
		friend BinSOStream& BinSOStream::operator<<( const MapCell& );
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

			friend BinSIStream& BinSIStream::operator>>( MapLayer& );
			friend BinSOStream& BinSOStream::operator<<( const MapLayer& );

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
