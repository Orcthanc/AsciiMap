#include <ncurses.h>

#include <iostream>

#include "ConsoleRenderer.hpp"

using namespace Pathfinder;

ConsoleRenderer::ConsoleRenderer( MapLayer&& ml ): 
		map_layer( std::move( ml )), 
		x_offset( 0 ), 
		y_offset( 0 ),
		running( true ){

	setlocale( LC_ALL, "" );
}

ConsoleRenderer::~ConsoleRenderer(){
	endwin();
}

void ConsoleRenderer::operator()(){
	initscr();
	cbreak();
	noecho();
	nonl();
	intrflush( stdscr, false );
	keypad( stdscr, true );
	curs_set( 0 );

	drawMapLayer();

	while( running ){
		handleUserInput();
	}
}

void ConsoleRenderer::handleUserInput(){
	switch( getch() ){
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
	}
}

void ConsoleRenderer::drawMapLayer(){
	clear();

	mvprintw( y_offset, x_offset, "%i", 12345 );

	std::cout << "Test" <<std::endl;

	refresh();
}
