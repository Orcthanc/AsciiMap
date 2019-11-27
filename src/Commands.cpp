#include "Commands.hpp"

#include "Common.hpp"
#include "Global.hpp"

using namespace Pathfinder;

static void loadfile( const char* filename ){
	GlobalData::get()->renderer->filename = filename;
	std::ifstream is( filename, std::ios::binary );
	is >> GlobalData::get()->renderer->map_layer;
}

#define bound_args( name, amount, min, max ) if( amount < min || amount > max ){ return cmd_ret_t{ false, false, #name " requires between " #min " and " #max " arguments." };}

cmd_ret_t quit( std::vector<const char*> v ){
	bound_args( quit, v.size(), 0, 0 );
	GlobalData::get()->renderer->running = false;
	return cmd_ret_t{ true, false, "Exiting" };
}

cmd_ret_t write( std::vector<const char*> v ){
	bound_args( write, v.size(), 0, 1 );
	std::string filename;
	if( v.size() == 1 )
		filename = GlobalData::get()->renderer->filename = v[0];
	else
		filename = GlobalData::get()->renderer->filename;

	if( filename.empty() )
		return cmd_ret_t{ false, false, "If there is no file open write needs a filename to write to" };
	std::ofstream os( filename, std::ios::binary );
	os << GlobalData::get()->renderer->map_layer;
	return cmd_ret_t{ true, false, "Written map to file: " + filename };
}

cmd_ret_t edit( std::vector<const char*> v ){
	bound_args( edit, v.size(), 0, 1 );
	GlobalData::get()->renderer->mode = Mode::Edit;
	if( v.size() == 0 )
		return cmd_ret_t{ true, false, "" };
	loadfile( v[0] );
	return cmd_ret_t{ true, true, ( std::string )"Opened file " + v[0] };
}

cmd_ret_t view( std::vector<const char*> v ){
	bound_args( view, v.size(), 0, 1 );
	GlobalData::get()->renderer->mode = Mode::View;
	if( v.size() == 0 )
		return cmd_ret_t{ true, false, "" };
	loadfile( v[0] );
	return cmd_ret_t{ true, true, ( std::string )"Opened file " + v[0] };
}

cmd_ret_t enew( std::vector<const char*> v ){
	bound_args( enew, v.size(), 0, 0 );
	GlobalData::get()->renderer->mode = Mode::Edit;
	GlobalData::get()->renderer->filename = "";
	GlobalData::get()->renderer->map_layer = {};
	return cmd_ret_t{ true, true, "Created new map. Use res to set dimensions" };
}

cmd_ret_t resize( std::vector<const char*> v ){
	bound_args( resize, v.size(), 1, 2 );
	int x, y;
	x = atoi( v[0] );
	if( v.size() == 2 )
		y = atoi( v[1] );
	else
		y = GlobalData::get()->renderer->map_layer.y_size;

	GlobalData::get()->renderer->map_layer.resize( x, y );
	return cmd_ret_t{ true, true, "Resized the map to: " + std::to_string( x ) + "x" + std::to_string( y ) };
}

cmd_ret_t help( std::vector<const char*> v ){
	Window& map = GlobalData::get()->renderer->windows.map;
	bound_args( help, v.size(), 0, 0 );
	int sx = ( map.x_size - 92 ) * 0.5; //TODO calculate dynamically
	int sy = ( map.y_size - GlobalData::get()->commands.help_text.size() - 2 ) * 0.5;
	werase( map );
	mvwprintw( map, sy, sx, "MapViewer written by Orcthanc Version " VERSION );
	auto it = GlobalData::get()->commands.help_text.cbegin();
	for( size_t i = 0; it != GlobalData::get()->commands.help_text.cend(); ++it, ++i ){
		mvwprintw( map, sy + 2 + i, sx, "%s", it->second.c_str() );
	}
	wrefresh( map );
	return cmd_ret_t{ true, false, ":help" };
}

void CommandList::registerAll(){
	Commands& cmds = GlobalData::get()->commands;
	cmds.insert( Mode::Exec, "quit", quit )
		.alias( Mode::Exec, "q" )
		.help( "q[uit]                                  : Exits the program" );

	cmds.insert( Mode::Exec, "write", write )
		.alias( Mode::Exec, "w" )
		.help( "w[rite] [filename]                      : Writes the current layer to filename or the currently open file" );

	cmds.insert( Mode::Exec, "edit", edit )
		.alias( Mode::Exec, "e" )
		.help( "e[dit] [filename]                       : Opens filename if given and switches to edit-mode" );

	cmds.insert( Mode::Exec, "view", view )
		.alias( Mode::Exec, "vie" )
		.help( "vie[w] [filename]                       : Opens filename if given and switches to view-mode" );

	cmds.insert( Mode::Exec, "enew", enew )
		.alias( Mode::Exec, "ene" )
		.help( "ene[w]                                  : Opens a new layer in edit-mode" );

	cmds.insert( Mode::Exec, "resize", resize )
		.alias( Mode::Exec, "res" )
		.help( "res[ize] x [y]                          : Resizes the map to x [* y]" );

	cmds.insert( Mode::Exec, "help", help )
		.help( "help                                    : Displays this help" );
}
