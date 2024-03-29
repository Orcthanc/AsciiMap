#include "Command.hpp"

using namespace Pathfinder;

insert_command_t& insert_command_t::alias( Mode m, std::string s ){
	cmds->alias( cmd, m, std::move( s ));
	return *this;
}

void Commands::alias( function_id_t id, Mode m, std::string s ){
	cmd_to_id.insert({ mangle( m, std::move( s )), id });
}

insert_command_t& insert_command_t::help( std::string s ){
	cmds->help( cmd, std::move( s ));
	return *this;
}

void Commands::help( function_id_t id, std::string s ){
	help_text.insert({ id, std::move( s )});
}
