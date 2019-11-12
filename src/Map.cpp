#include <fstream>

#include "Map.hpp"

using namespace Pathfinder;

MapLayer::MapLayer( const std::string& filename ){
	std::fstream file( filename, std::ios::binary );

	file >> x_size >> y_size;

	cells = new MapCell[x_size * y_size];

	//TODO
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

