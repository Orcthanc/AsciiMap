#include "Commands.hpp"

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
}
