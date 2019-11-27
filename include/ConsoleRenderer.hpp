#pragma once

#include <ncurses.h>

#include "DrawUtils.hpp"
#include "Map.hpp"

namespace Pathfinder {
	struct Commands;

	enum class Mode {
		None,	//On startup when no file is open
		View,	//Self explanatory
		Edit,	//Self explanatory
		Exec,	//Dummy mode (should not be entered) to signal a colon command
	};

	extern const char* ModeToString( Mode m );
	extern short ModeToColor( Mode m );

#define COLOR_LIGHT_GREEN ( COLORS - 1 )
#define COLOR_DARK_GREEN ( COLORS - 2 )
#define COLOR_LIGHT_GREY ( COLORS - 3 )
#define COLOR_DARK_GREY ( COLORS - 4 )
#define COLOR_DARK_BLACK ( COLORS - 5 )

#define PAIR_DGREY_LGREEN 1
#define PAIR_DGREY_DGREEN 2
#define PAIR_LGREEN_LGREY 3
#define PAIR_DGREEN_LGREY 4
#define PAIR_LGRAY_DGRAY 5
#define PAIR_LGREEN_DGREY 6
#define PAIR_DGREEN_DGREY 7
#define PAIR_BLACK_LGREEN 8
#define PAIR_BLACK_DGREEN 9

	struct Window {
		Window(): n_window{ nullptr }, x_size{ 0 }, y_size{ 0 }{}
		Window( int x, int y, int x_size, int y_size );

		Window( const Window& ) = delete;
		Window( Window&& w ){
			n_window = w.n_window;
			x_size = w.x_size;
			y_size = w.y_size;
			w.n_window = nullptr;
		}

		~Window();

		Window& operator=( const Window& ) = delete;
		Window& operator=( Window&& w ){
			n_window = w.n_window;
			x_size = w.x_size;
			y_size = w.y_size;
			w.n_window = nullptr;
			return *this;
		}
		operator WINDOW*();

		WINDOW* n_window;
		uint32_t x_size;
		uint32_t y_size;
	};

	struct Windows {
		Window map;
		Window infobar;
		Window bottom;
	};

	class ConsoleRenderer {
		#define MAX_INPUT_SIZE 256

		public:
			ConsoleRenderer( MapLayer&& ml );
			~ConsoleRenderer();

			void operator()();

			void drawMapLayer();
			void drawFooter();
			void handleUserInput();
			bool handleColon();

			MapLayer map_layer;
			int x_size, y_size;
			int x_offset, y_offset;
			int scale;
			bool running;
			char input[MAX_INPUT_SIZE];
			Mode mode;
			Windows windows;
			std::string filename;

	};
}
