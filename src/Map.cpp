#include <fstream>

#include "Map.hpp"

using namespace Pathfinder;

//TODO binary in/out
std::istream& Pathfinder::operator>>( std::istream& is, MapLayer& m ){
	/*
	is >> m.x_size >> m.y_size;

	m.cells = new MapCell[m.x_size * m.y_size];

	for( size_t i = 0; i < m.x_size * m.y_size; ++i ){
		is >> m.cells[i];
	}

	return is;
	*/

	BinSIStream( is ) >> m;
	return is;
}

BinSIStream& BinSIStream::operator>>( MapLayer& m ){
	*this >> m.x_size >> m.y_size;

	m.cells = new MapCell[m.x_size * m.y_size];

	for( size_t i = 0; i < m.x_size * m.y_size; ++i ){
		*this >> m.cells[i];
	}

	return *this;
}

BinSOStream& BinSOStream::operator<<( const MapLayer& m ){
	*this << m.x_size << m.y_size;

	for( size_t i = 0; i < m.x_size * m.y_size; ++i ){
		*this << m.cells[i];
	}

	return *this;
}

std::ostream& Pathfinder::operator<<( std::ostream& os, const MapLayer& m ){
	/*
	os << m.x_size << m.y_size;

	for( size_t i = 0; i < m.x_size * m.y_size; ++i ){
		os << m.cells[i];
	}
*/
	BinSOStream( os ) << m;
	return os;

}

MapLayer::MapLayer( const std::vector<MapCell>& cells, uint32_t x_size, uint32_t y_size ): x_size( x_size ), y_size( y_size ){
	this->cells = new MapCell[x_size * y_size];

	for( size_t i = 0; i < x_size * y_size; ++i ){
		this->cells[i] = cells[i];
	}
}

MapLayer::MapLayer( MapLayer&& m ){
	cells = m.cells;
	x_size = m.x_size;
	y_size = m.y_size;
	m.cells = nullptr;
}

MapLayer& MapLayer::operator=( MapLayer&& m ){
	cells = m.cells;
	x_size = m.x_size;
	y_size = m.y_size;
	m.cells = nullptr;
	return *this;
}

std::istream& Pathfinder::operator>>( std::istream& i, MapCell& m ){
	/*
	return i >> m.area_code >> m.walls.north >> m.walls.west >> m.walls.current;
	*/
	BinSIStream( i ) >> m;
	return i;
}

BinSIStream& BinSIStream::operator>>( MapCell& m ){
	return *this >> m.area_code >> m.walls.north >> m.walls.west >> m.walls.current;
}

std::ostream& Pathfinder::operator<<( std::ostream& o, const MapCell& m ){
	/*
	return o << m.area_code << m.walls.north << m.walls.west << m.walls.current;
	*/
	BinSOStream( o ) << m;
	return o;
}

BinSOStream& BinSOStream::operator<<( const MapCell& m ){
	return *this << m.area_code << m.walls.north << m.walls.west << m.walls.current;
}

MapLayer::~MapLayer(){
	delete[] cells;
}

MapCell& MapLayer::at( uint32_t x, uint32_t y ) noexcept {
	return cells[x + y * x_size];
}

const MapCell& MapLayer::at( uint32_t x, uint32_t y ) const noexcept {
	return cells[x + y * x_size];
}

MapCell& MapLayer::operator[]( const std::array<uint32_t, 2>& xy ) noexcept {
	return at( xy[0], xy[1] );
}

const MapCell& MapLayer::operator[]( const std::array<uint32_t, 2>& xy ) const noexcept {
	return at( xy[0], xy[1] );
}
