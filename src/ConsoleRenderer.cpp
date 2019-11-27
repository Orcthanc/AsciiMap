#include <ncurses.h>
#include <string.h>

#include <iostream>
#include <fstream>

#include "Common.hpp"
#include "ConsoleRenderer.hpp"
#include "Commands.hpp"
#include "Global.hpp"

using namespace Pathfinder;

Window::Window( int x, int y, int x_size, int y_size ){
	n_window = newwin( y_size, x_size, y, x );
	if( !n_window )
		std::cerr << "Could not create window at: " << x << "; " << y << std::endl;
	getmaxyx( n_window, this->y_size, this->x_size );
}

Window::~Window(){
	if( n_window )
		delwin( n_window );
}

Window::operator WINDOW*(){
	return n_window;
}

ConsoleRenderer::ConsoleRenderer( MapLayer&& ml ):
		map_layer( std::move( ml )),
		x_offset( 0 ),
		y_offset( 0 ),
		scale( 4 ),
		running( true ),
		mode( Mode::None ),
		filename()
		{

	setlocale( LC_ALL, "" );
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

	getmaxyx( stdscr, y_size, x_size );

	windows.map = { 0, 0, 0, y_size - 2 };
	windows.infobar = { 0, y_size - 2, 0, 1 };
	windows.bottom = { 0, y_size - 1, 0, 0 };

	//TODO replace stdscr

	CommandList::registerAll();
}

ConsoleRenderer::~ConsoleRenderer(){
	standend();
	endwin();
}

void ConsoleRenderer::operator()(){
	GlobalData::get()->renderer = this;
	refresh();

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

	drawFooter();
	//drawMapLayer();

	/*
	wborder( windows.map, '+', '+', '+', '+', '+', '+', '+', '+' );
	wborder( windows.infobar, '-', '-', '-', '-', '-', '-', '-', '-' );
	wborder( windows.bottom, '.', '.', '.', '.', '.', '.', '.', '.' );
	*/

	wrefresh( windows.map );
	wrefresh( windows.infobar );
	wrefresh( windows.bottom );

	while( running ){
		handleUserInput();
	}
}

void ConsoleRenderer::handleUserInput(){
	int c;
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
		case KEY_RESIZE:
			//TODO
			break;
		default:
			mvwprintw( windows.bottom, 0, windows.bottom.x_size - 5, "% 5i", c );
			break;
	}
	drawFooter();
}

void ConsoleRenderer::drawFooter(){
	int color;
	int x = 0;

	switch( mode ){
		case Mode::None:
		case Mode::View:
			wattron( windows.infobar, color = COLOR_PAIR( PAIR_BLACK_LGREEN ));
			break;
		case Mode::Edit:
		case Mode::Exec:
			wattron( windows.infobar, color = COLOR_PAIR( PAIR_BLACK_DGREEN ));
			break;
	}
	wmove( windows.infobar, 0, 0 );

	wattron( windows.infobar, A_BOLD );
	wprintw( windows.infobar, " %s ", ModeToString( mode ));
	x += snprintf( nullptr, 0, " %s ", ModeToString( mode ));

	wattroff( windows.infobar, A_BOLD );
	wattroff( windows.infobar, color );

	switch( mode ){
		case Mode::None:
		case Mode::View:
			wattron( windows.infobar, color = COLOR_PAIR( PAIR_LGREEN_LGREY ));
			break;
		case Mode::Edit:
		case Mode::Exec:
			wattron( windows.infobar, color = COLOR_PAIR( PAIR_DGREEN_LGREY ));
			break;
	}

	wprintw( windows.infobar, " x_offset: %i, y_offset: %i, scale: %i ", x_offset, y_offset, scale );
	x += snprintf( nullptr, 0, " x_offset: %i, y_offset: %i, scale: %i ", x_offset, y_offset, scale );

	wattroff( windows.infobar, color );
	wattron( windows.infobar, color = COLOR_PAIR( PAIR_LGRAY_DGRAY ));

	wprintw( windows.infobar, "" );
	x += 1;

	wattroff( windows.infobar, color );

	switch( mode ){
		case Mode::None:
		case Mode::View:
			wattron( windows.infobar, color = COLOR_PAIR( PAIR_LGREEN_DGREY ));
			break;
		case Mode::Edit:
		case Mode::Exec:
			wattron( windows.infobar, color = COLOR_PAIR( PAIR_DGREEN_DGREY ));
			break;

	}
	wprintw( windows.infobar, " %s", filename == "" ? "[New file]" : filename.c_str());
	x += filename.empty() ? 10 : snprintf( nullptr, 0, " %s", filename.c_str() );

	wprintw( windows.infobar, "% *s", x_size - x + 2, "" );

	wattroff( windows.infobar, color );

	wrefresh( windows.infobar.n_window );
}

#define Y_SCALE_FAC 0.5f
#define SCREEN_X ( x * scale + x_offset )
#define SCREEN_Y ( y * std::max( (int)( scale * Y_SCALE_FAC ), 1 ) + y_offset )

void ConsoleRenderer::drawMapLayer(){
	werase( windows.map );

	for( uint32_t y = 0; y < map_layer.y_size; ++y ){
		for( uint32_t x = 0; x < map_layer.x_size; ++x ){
			wmove( windows.map, SCREEN_Y, SCREEN_X );
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

			if( SCREEN_X >= 0 && SCREEN_X < static_cast<unsigned>( windows.map.x_size ) && SCREEN_Y >= 0 && SCREEN_Y < static_cast<unsigned>( windows.map.y_size )){
				wprintw( windows.map, "%s", dir_to_chars( static_cast<Wall::Wall>( draw )).c_str() );
			}

			for( int x2 = 1; x2 < scale; ++x2 ){
				if( SCREEN_X + x2 >= 0 && SCREEN_X + x2 < static_cast<unsigned>( windows.map.x_size ) && SCREEN_Y >= 0 && SCREEN_Y < static_cast<unsigned>( windows.map.y_size )){
					mvwprintw( windows.map, SCREEN_Y, SCREEN_X + x2, "%s", dir_to_chars( static_cast<Wall::Wall>( line & 0b11000011 )).c_str() );
				}
			}

			for( int y2 = 1; y2 < scale * Y_SCALE_FAC; ++y2 ){
				if( SCREEN_X >= 0 && SCREEN_X < static_cast<unsigned>( windows.map.x_size ) && SCREEN_Y + y2 >= 0 && SCREEN_Y + y2 < static_cast<unsigned>( windows.map.y_size )){
					mvwprintw( windows.map, SCREEN_Y + y2, SCREEN_X, "%s", dir_to_chars( static_cast<Wall::Wall>( line & 0b00111100 )).c_str() );
				}
			}
		}
	}
	wrefresh( windows.map );
}

#undef SCREEN_X
#undef SCREEN_Y

bool ConsoleRenderer::handleColon(){
	int curr;
	memset( input, 0, MAX_INPUT_SIZE );

	werase( windows.bottom );
	curs_set( 1 );

	wrefresh( windows.bottom );

	for( int i = 0; ; ++i ){
		mvwprintw( windows.bottom, 0, 0, ":%s ", input );
		wmove( stdscr, y_size - 1, i + 1 );
		wrefresh( windows.bottom );
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

	char* temp;
	std::vector<const char*> args;

	while(( temp = strtok( nullptr, " " ))){
		args.push_back( temp );
	}

	auto t = GlobalData::get()->commands.find_and_call( Mode::Exec, command, args );

	mvwprintw( windows.bottom, 0, 0, "%s", t.message.c_str() );
	wrefresh( windows.bottom );

	return t.redraw && t.success;

	/*

	if( command == nullptr )
		return false;
	else if( !strcmp( command, "write" ) || !strcmp( command, "w" )){
		const char* arg = strtok( nullptr, " " );
		if( arg == nullptr ){
			if( GlobalData::get()->filename == "" ){
				mvwprintw( windows.bottom, 0, 0, "Missing parameter file for function write" );
				return false;
			}
			arg = GlobalData::get()->filename.c_str();
		}
		std::ofstream os( arg, std::ios::binary );
		os << map_layer;
		mvwprintw( windows.bottom, 0, 0, "Map of size %ux%u written to \"%s\"", map_layer.x_size, map_layer.y_size, GlobalData::get()->filename.c_str() );
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
		//TODO doesn't delete old layer
		map_layer = MapLayer();
		GlobalData::get()->filename = "";
		return true;
	} else if( !strcmp( command, "resize" ) || !strcmp( command, "res" )){
		int x, y;
		const char* arg = strtok( nullptr, " " );
		if( arg == nullptr ){
			mvwprintw( windows.bottom, 0, 0, "Missing argument to function res[ize] x [y]" );
		}
		x = atoi( arg );
		arg = strtok( nullptr, " " );
		if( arg == nullptr )
			y = map_layer.y_size;
		else
			y = atoi( arg );
		map_layer.resize( x, y );
		mvwprintw( windows.bottom, 0, 0, "Resized map to %ix%i", x, y );
		return false;
	} else if( !strcmp( command, "help" )){
		werase( windows.map );
		constexpr int width = 97, height = 17;
		static const std::vector<const char*> lines{
				"MapViewer written by Orcthanc Version " VERSION,
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
	/			" - :w[rite] [filename]        Saves the map to filename, or the last opened file if none specified",
				" - :vie[w] [filename]         Switches to view-mode and opens filename if given",
	/			" - :e[dit] [filename]         Switches to edit-mode and opens filename if given",
				" - :ene[w]                    Switches to edit-mode and creates a new map",
				" - :res[ize] x [y]            Resizes the map-layer to the dimension x*y",
		};

		for( size_t i = 0; i < lines.size(); ++i ){
			mvwprintw( windows.map, ( windows.map.y_size - height ) * 0.5 + i, ( windows.map.x_size - width ) * 0.5, "%s", lines[i] );
		}
		wrefresh( windows.map );
		return false;
	} else {
		mvwprintw( windows.bottom, 0, 0, "Unrecognized command %s", command );
		return false;
	}
	*/
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

	throw std::runtime_error( "Invalid mode a: " + std::to_string( static_cast<std::underlying_type_t<Mode>>( m )));
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
