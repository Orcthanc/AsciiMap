#include <ncurses.h>
#include <string.h>

#include <iostream>
#include <fstream>

#include "Common.hpp"
#include "ConsoleRenderer.hpp"
#include "Commands.hpp"
#include "Global.hpp"

using namespace Pathfinder;

ConsoleRenderer::ConsoleRenderer( MapLayer&& ml ):
		map_layer( std::move( ml )),
		x_offset( 0 ),
		y_offset( 0 ),
		scale( 4 ),
		running( true )
		{

	getmaxyx( stdscr, y_size, x_size );
	setlocale( LC_ALL, "" );

	CommandList::registerAll();
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
	init_color( COLOR_LIGHT_GREY, COL( 0x3D ), COL( 0x3D ), COL( 0x3D ));
	init_color( COLOR_DARK_GREY, COL( 0x22 ), COL( 0x22 ), COL( 0x22 ));
	init_color( COLOR_DARK_BLACK, COL( 0x04 ), COL( 0x04 ), COL( 0x04 ));

	init_pair( PAIR_DGREY_LGREEN, COLOR_DARK_GREY, COLOR_LIGHT_GREEN );
	init_pair( PAIR_DGREY_DGREEN, COLOR_DARK_GREY, COLOR_DARK_GREEN );
	init_pair( PAIR_LGREEN_LGREY, COLOR_LIGHT_GREEN, COLOR_LIGHT_GREY );
	init_pair( PAIR_DGREEN_LGREY, COLOR_DARK_GREEN, COLOR_LIGHT_GREY );
	init_pair( PAIR_LGRAY_DGRAY, COLOR_LIGHT_GREY, COLOR_DARK_GREY );
	init_pair( PAIR_LGREEN_DGREY, COLOR_LIGHT_GREEN, COLOR_DARK_GREY );
	init_pair( PAIR_DGREEN_DGREY, COLOR_DARK_GREEN, COLOR_DARK_GREY );
	init_pair( PAIR_BLACK_LGREEN, COLOR_DARK_BLACK, COLOR_LIGHT_GREEN );
	init_pair( PAIR_BLACK_DGREEN, COLOR_DARK_BLACK, COLOR_DARK_GREEN );

	getmaxyx( stdscr, y_size, x_size );

	drawMapLayer();
	drawFooter();

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
			if( mode == Mode::None )
				break;
			--x_offset;
			drawMapLayer();
			break;
		case 'd':
		case KEY_RIGHT:
			if( mode == Mode::None )
				break;
			++x_offset;
			drawMapLayer();
			break;
		case 'w':
		case KEY_UP:
			if( mode == Mode::None )
				break;
			--y_offset;
			drawMapLayer();
			break;
		case 's':
		case KEY_DOWN:
			if( mode == Mode::None )
				break;
			++y_offset;
			drawMapLayer();
			break;
		case '+':
			if( mode == Mode::None )
				break;
			++scale;
			drawMapLayer();
			break;
		case '-':
			if( mode == Mode::None )
				break;
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
	refresh();
}

void ConsoleRenderer::drawFooter(){
	//mvprintw( y_size - 1, 0, "x_offset: %i, y_offset: %i, scale: %i", x_offset, y_offset, scale );
	int color;
	int x = 0;

	switch( mode ){
		case Mode::None:
		case Mode::View:
			attron( color = COLOR_PAIR( PAIR_BLACK_LGREEN ));
			break;
		case Mode::Edit:
		case Mode::Exec:
			attron( color = COLOR_PAIR( PAIR_BLACK_DGREEN ));
			break;
	}
	wmove( stdscr, y_size - 2, 0 );

	attron( A_BOLD );
	printw( " %s ", ModeToString( mode ));
	x += snprintf( nullptr, 0, " %s ", ModeToString( mode ));

	attroff( A_BOLD );
	attroff( color );

	switch( mode ){
		case Mode::None:
		case Mode::View:
			attron( color = COLOR_PAIR( PAIR_LGREEN_LGREY ));
			break;
		case Mode::Edit:
		case Mode::Exec:
			attron( color = COLOR_PAIR( PAIR_DGREEN_LGREY ));
			break;
	}

	printw( " x_offset: %i, y_offset: %i, scale: %i ", x_offset, y_offset, scale );
	x += snprintf( nullptr, 0, " x_offset: %i, y_offset: %i, scale: %i ", x_offset, y_offset, scale );

	attroff( color );
	attron( color = COLOR_PAIR( PAIR_LGRAY_DGRAY ));

	printw( "" );
	x += 1;

	attroff( color );

	switch( mode ){
		case Mode::None:
		case Mode::View:
			attron( color = COLOR_PAIR( PAIR_LGREEN_DGREY ));
			break;
		case Mode::Edit:
		case Mode::Exec:
			attron( color = COLOR_PAIR( PAIR_DGREEN_DGREY ));
			break;

	}
	printw( " %s", GlobalData::get()->filename == "" ? "[New file]" : GlobalData::get()->filename.c_str());
	x += GlobalData::get()->filename.empty() ? 10 : snprintf( nullptr, 0, " %s", GlobalData::get()->filename.c_str() );

	printw( "% *s", x_size - x + 2, "" );

	attroff( color );
}

#define Y_SCALE_FAC 0.5f
#define SCREEN_X ( x * scale + x_offset )
#define SCREEN_Y ( y * std::max( (int)( scale * Y_SCALE_FAC ), 1 ) + y_offset )

void ConsoleRenderer::drawMapLayer(){
	erase();

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
			if( SCREEN_X >= 0 && SCREEN_X < static_cast<unsigned>( x_size ) && SCREEN_Y >= 0 && SCREEN_Y < static_cast<unsigned>( y_size - 2 )){
				printw( "%s", dir_to_chars( static_cast<Wall::Wall>( draw )).c_str() );
			}

			for( int x2 = 1; x2 < scale; ++x2 ){
				if( SCREEN_X + x2 >= 0 && SCREEN_X + x2 < static_cast<unsigned>( x_size ) && SCREEN_Y >= 0 && SCREEN_Y < static_cast<unsigned>( y_size - 2 )){
					mvprintw( SCREEN_Y, SCREEN_X + x2, "%s", dir_to_chars( static_cast<Wall::Wall>( line & 0b11000011 )).c_str() );
				}
			}

			for( int y2 = 1; y2 < scale * Y_SCALE_FAC; ++y2 ){
				if( SCREEN_X >= 0 && SCREEN_X < static_cast<unsigned>( x_size ) && SCREEN_Y + y2 >= 0 && SCREEN_Y + y2 < static_cast<unsigned>( y_size - 2 )){
					mvprintw( SCREEN_Y + y2, SCREEN_X, "%s", dir_to_chars( static_cast<Wall::Wall>( line & 0b00111100 )).c_str() );
				}
			}
		}
	}
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
			if( GlobalData::get()->filename == "" ){
				mvprintw( y_size - 1, 0, "Missing parameter file for function write" );
				refresh();
				return false;
			}
			arg = GlobalData::get()->filename.c_str();
		}
		std::ofstream os( arg, std::ios::binary );
		os << map_layer;
		mvprintw( y_size - 1, 0, "Map of size %ux%u written to \"%s\"", map_layer.x_size, map_layer.y_size, GlobalData::get()->filename.c_str() );
		return false;
	} else if( !strcmp( command, "view" ) || !strcmp( command, "vie" )){
		const char* arg = strtok( nullptr, " " );
		if( arg != nullptr ){
			GlobalData::get()->filename = arg;
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
			GlobalData::get()->filename = arg;
			std::ifstream is( arg, std::ios::binary );
			is >> map_layer;
		}
		mode = Mode::Edit;
		return true;
	} else if( !strcmp( command, "enew" ) || !strcmp( command, "ene" )){
		mode = Mode::Edit;
		map_layer = MapLayer();
		GlobalData::get()->filename = "";
		return true;
	} else if( !strcmp( command, "resize" ) || !strcmp( command, "res" )){
		int x, y;
		const char* arg = strtok( nullptr, " " );
		if( arg == nullptr ){
			mvprintw( y_size - 1, 0, "Missing argument to function res[ize] x [y]" );
		}
		x = atoi( arg );
		arg = strtok( nullptr, " " );
		if( arg == nullptr )
			y = map_layer.y_size;
		else
			y = atoi( arg );
		map_layer.resize( x, y );
		mvprintw( y_size - 1, 0, "Resized map to %ix%i", x, y );
		return false;
	} else if( !strcmp( command, "help" )){
		erase();
		constexpr int width = 97, height = 17;
//		mvprintw(( y_size - height ) * 0.5, ( x_size - width ) * 0.5, "" );
		static const std::vector<const char*> lines{
				"MapViewer written by Orcthanc Version " VERSION "",
				"",
				"Available commands:",
				" - w                          Move the map up one char",
				" - up_arrow                   See above",
				" - s                          Move the map down one char",
				" - down_arrow                 See above",
				" - a                          Move the map left one char",
				" - left_arrow                 See above",
				" - d                          Move the map right one char",
				" - right_arrow                See above",
				" - :help                      Displays this help",
				" - :w[rite] [filename]        Saves the map to filename, or the last opened file if none specified",
				" - :vie[w] [filename]         Switches to view-mode and opens filename if given",
				" - :e[dit] [filename]         Switches to edit-mode and opens filename if given",
				" - :ene[w]                    Switches to edit-mode and creates a new map",
				" - :res[ize] x [y]            Resizes the map-layer to the dimension x*y",
		};

		for( size_t i = 0; i < lines.size(); ++i ){
			mvprintw(( y_size - height ) * 0.5 + i, ( x_size - width ) * 0.5, "%s", lines[i] );
		}
		return false;
	} else {
		mvprintw( y_size - 1, 0, "Unrecognized command %s", command );
		return false;
	}
}

const char* Pathfinder::ModeToString( Mode m ){
	switch( m ){
		case Mode::None:
			return "----";
		case Mode::View:
			return "VIEW";
		case Mode::Edit:
			return "EDIT";
		case Mode::Exec:
			return "Exec";
	}

	throw std::runtime_error( "Invalid mode: " + std::to_string( static_cast<std::underlying_type_t<Mode>>( m )));
}

short Pathfinder::ModeToColor( Mode m ){
	switch( m ){
		case Mode::None:
			return COLOR_CYAN;
		case Mode::View:
			return COLOR_LIGHT_GREEN;
		case Mode::Edit:
			return COLOR_DARK_GREEN;
		case Mode::Exec:
			return COLOR_MAGENTA;
	}

	throw std::runtime_error( "Invalid mode: " + std::to_string( static_cast<std::underlying_type_t<Mode>>( m )));
}
