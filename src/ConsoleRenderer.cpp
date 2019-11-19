#include <ncurses.h>
#include <string.h>

#include <iostream>
#include <fstream>

#include "ConsoleRenderer.hpp"

using namespace Pathfinder;

ConsoleRenderer::ConsoleRenderer( MapLayer&& ml ):
		map_layer( std::move( ml )),
		x_offset( 0 ),
		y_offset( 0 ),
		scale( 4 ),
		running( true ),
		filename(){

	getmaxyx( stdscr, y_size, x_size );
	setlocale( LC_ALL, "" );
}

ConsoleRenderer::~ConsoleRenderer(){
	standend();
	endwin();
}

void ConsoleRenderer::operator()(){
	initscr();
	start_color();
	cbreak();
	noecho();
	nonl();
	intrflush( stdscr, false );
	keypad( stdscr, true );
	set_escdelay( 20 );
	curs_set( 0 );
	use_default_colors();

#define COL( i ) ( i * 1000 / 0xff )

	init_color( COLOR_LIGHT_GREEN, COL( 0xCA ), COL( 0xE6 ), COL( 0x82 ));
	init_color( COLOR_DARK_GREEN, COL( 0xFD ), COL( 0xE7 ), COL( 0x6E ));
	init_color( COLOR_LIGHT_GREY, COL( 0x32 ), COL( 0x32 ), COL( 0x2F ));
	init_color( COLOR_DARK_GREY, COL( 0x24 ), COL( 0x24 ), COL( 0x24 ));

	init_pair( PAIR_DGREY_LGREEN, COLOR_DARK_GREY, COLOR_LIGHT_GREEN );
	init_pair( PAIR_DGREY_DGREEN, COLOR_DARK_GREY, COLOR_DARK_GREEN );
	init_pair( PAIR_LGREEN_LGREY, COLOR_LIGHT_GREEN, COLOR_LIGHT_GREY );
	init_pair( PAIR_DGREEN_LGREY, COLOR_DARK_GREEN, COLOR_LIGHT_GREY );
	init_pair( PAIR_LGRAY_DGRAY, COLOR_LIGHT_GREY, COLOR_DARK_GREY );

	drawMapLayer();

	while( running ){
		handleUserInput();
	}
}

void ConsoleRenderer::handleUserInput(){
	int c;
	getmaxyx( stdscr, y_size, x_size );
	switch( c = getch() ){
		case 27: //ESC
			running = false;
			break;
		case 'a':
		case KEY_LEFT:
			--x_offset;
			drawMapLayer();
			break;
		case 'd':
		case KEY_RIGHT:
			++x_offset;
			drawMapLayer();
			break;
		case 'w':
		case KEY_UP:
			--y_offset;
			drawMapLayer();
			break;
		case 's':
		case KEY_DOWN:
			++y_offset;
			drawMapLayer();
			break;
		case '+':
			++scale;
			drawMapLayer();
			break;
		case '-':
			if( --scale == 0 )
				scale = 1;
			else
				drawMapLayer();
			break;
		case ':':
			if( handleColon() )
				drawMapLayer();
			break;
		default:
			mvprintw( y_size - 1, x_size - 5, "% 5i", c );
			break;
	}
	drawFooter();
}

void ConsoleRenderer::drawFooter(){
	//mvprintw( y_size - 1, 0, "x_offset: %i, y_offset: %i, scale: %i", x_offset, y_offset, scale );
	int color;

	switch( mode ){
		case Mode::View:
			attron( color = COLOR_PAIR( PAIR_DGREY_LGREEN ));
			break;
		case Mode::Edit:
			attron( color = COLOR_PAIR( PAIR_DGREY_DGREEN ));
			break;
	}
	wmove( stdscr, y_size - 2, 0 );

	attron( A_BOLD );
	printw( " %s ", ModeToString( mode ));

	attroff( A_BOLD );
	attroff( color );

	switch( mode ){
		case Mode::View:
			attron( color = COLOR_PAIR( PAIR_LGREEN_LGREY ));
			break;
		case Mode::Edit:
			attron( color = COLOR_PAIR( PAIR_DGREEN_LGREY ));
			break;
	}

	printw( " x_offset: %i, y_offset: %i, scale: %i", x_offset, y_offset, scale );

	attroff( color );
}

#define Y_SCALE_FAC 0.5f
#define SCREEN_X ( x * scale + x_offset )
#define SCREEN_Y ( y * std::max( (int)( scale * Y_SCALE_FAC ), 1 ) + y_offset )

void ConsoleRenderer::drawMapLayer(){
	clear();

	for( uint32_t y = 0; y < map_layer.y_size; ++y ){
		for( uint32_t x = 0; x < map_layer.x_size; ++x ){
			wmove( stdscr, SCREEN_Y, SCREEN_X );
			std::underlying_type_t<Wall::Wall> draw = 0;

			using WallType_t = std::underlying_type_t<WallType>;

			WallType_t line = 0;
			//Draw top left corner
			{
				if( map_layer[{ x, y }].walls.north == static_cast<WallType_t>( WallType::wall )){
					draw += 0b10;
					line += 0b10000010;
				}
				if( map_layer[{ x, y }].walls.north == static_cast<WallType_t>( WallType::door )){
					draw += 0b01;
					line += 0b01000001;
				}

				if( map_layer[{ x, y }].walls.west == static_cast<WallType_t>( WallType::wall )){
					draw += 0b100000;
					line += 0b00101000;
				}
				if( map_layer[{ x, y }].walls.west == static_cast<WallType_t>( WallType::door )){
					draw += 0b010000;
					line += 0b00010100;
				}

				if( x > 0 && map_layer[{ x - 1, y }].walls.north == static_cast<WallType_t>( WallType::wall ))
					draw += 0b10000000;
				if( x > 0 && map_layer[{ x - 1, y }].walls.north == static_cast<WallType_t>( WallType::door ))
					draw += 0b01000000;

				if( y > 0 && map_layer[{ x, y - 1 }].walls.west == static_cast<WallType_t>( WallType::wall ))
					draw += 0b1000;
				if( y > 0 && map_layer[{ x, y - 1 }].walls.west == static_cast<WallType_t>( WallType::door ))
				draw += 0b0100;
			}
			if( SCREEN_X >= 0 && SCREEN_X < static_cast<unsigned>( x_size ) && SCREEN_Y >= 0 && SCREEN_Y < static_cast<unsigned>( y_size )){
				printw( "%s", dir_to_chars( static_cast<Wall::Wall>( draw )).c_str() );
			}

			for( int x2 = 1; x2 < scale; ++x2 ){
				if( SCREEN_X + x2 >= 0 && SCREEN_X + x2 < static_cast<unsigned>( x_size ) && SCREEN_Y >= 0 && SCREEN_Y < static_cast<unsigned>( y_size )){
					mvprintw( SCREEN_Y, SCREEN_X + x2, "%s", dir_to_chars( static_cast<Wall::Wall>( line & 0b11000011 )).c_str() );
				}
			}

			for( int y2 = 1; y2 < scale * Y_SCALE_FAC; ++y2 ){
				if( SCREEN_X >= 0 && SCREEN_X < static_cast<unsigned>( x_size ) && SCREEN_Y + y2 >= 0 && SCREEN_Y + y2 < static_cast<unsigned>( y_size )){
					mvprintw( SCREEN_Y + y2, SCREEN_X, "%s", dir_to_chars( static_cast<Wall::Wall>( line & 0b00111100 )).c_str() );
				}
			}
		}
	}

	refresh();
}

#undef SCREEN_X
#undef SCREEN_Y

bool ConsoleRenderer::handleColon(){
	int curr;
	memset( input, 0, MAX_INPUT_SIZE );

	mvprintw( y_size - 1, 0, "%*.s", x_size - 1, "" );
	curs_set( 1 );

	for( int i = 0; ; ++i ){
		mvprintw( y_size - 1, 0, ":%s ", input );
		wmove( stdscr, y_size - 1, i + 1 );
		refresh();
		if(( curr = getch()) == '\r' )
			break;
		if( curr == KEY_BACKSPACE ){
			if( i == 0 ){
				curs_set( 0 );
				return true;
			}
			input[--i] = 0;
			--i;
		} else if( curr == 27 ){ //Escape
			curs_set( 0 );
			return true;
		} else if( curr < 256 ){
			input[i] = curr;
		} else {
			std::cerr << "Character exceeding max char detected (" << curr << ")" << std::endl;
		}
	}

	curs_set( 0 );
	char* command = strtok( input, " " );

	if( command == nullptr )
		return false;
	else if( !strcmp( command, "write" ) || !strcmp( command, "w" )){
		const char* arg = strtok( nullptr, " " );
		if( arg == nullptr ){
			if( filename == "" ){
				mvprintw( y_size - 1, 0, "Missing parameter file for function write" );
				refresh();
				return false;
			}
			arg = filename.c_str();
		}
		std::ofstream os( arg, std::ios::binary );
		os << map_layer;
		return false;
	} else if( !strcmp( command, "view" ) || !strcmp( command, "vie" )){
		const char* arg = strtok( nullptr, " " );
		if( arg != nullptr ){
			filename = arg;
			std::ifstream is( arg, std::ios::binary );
			is >> map_layer;
		}
		mode = Mode::View;
		return true;
	} else if( !strcmp( command, "quit" ) || !strcmp( command, "q" )){
		running = false;
		return false;
	} else if( !strcmp( command, "edit" ) || !strcmp( command, "e" )){
		const char* arg = strtok( nullptr, " " );
		if( arg != nullptr ){
			filename = arg;
			std::ifstream is( arg, std::ios::binary );
			is >> map_layer;
		}
		mode = Mode::Edit;
		return true;
	} else {
		mvprintw( y_size - 1, 0, "Unrecognized command %s", command );
		return false;
	}
}

const char* Pathfinder::ModeToString( Mode m ){
	switch( m ){
		case Mode::View:
			return "VIEW";
		case Mode::Edit:
			return "EDIT";
	}

	throw std::runtime_error( "Invalid mode: " + std::to_string( static_cast<std::underlying_type_t<Mode>>( m )));
}

short Pathfinder::ModeToColor( Mode m ){
	switch( m ){
		case Mode::View:
			return COLOR_LIGHT_GREEN;
		case Mode::Edit:
			return COLOR_DARK_GREEN;
	}

	throw std::runtime_error( "Invalid mode: " + std::to_string( static_cast<std::underlying_type_t<Mode>>( m )));
}
