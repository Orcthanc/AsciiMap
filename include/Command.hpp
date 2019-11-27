#pragma once

#include <map>
#include "ConsoleRenderer.hpp"

namespace Pathfinder {
	struct Commands;

	using function_id_t = size_t;

	inline std::string mangle( Mode m, std::string name, size_t argc ){
		return std::to_string( static_cast<std::underlying_type_t<Mode>>( m )) + name + std::to_string( argc );
	}

	struct insert_command_t {
		insert_command_t& alias( Mode m, std::string s, size_t argc );
		insert_command_t& help( std::string s );

		Commands* cmds;
		function_id_t cmd;
	};

	struct cmd_ret_t {
		bool success, redraw;
		const char* message;	//Displayed in lower left
	};

	using function_t = cmd_ret_t (*)( void );


	struct Commands {

		public:
			template<typename T, typename Name>
			auto insert( Mode m, Name&& s, size_t argc, T&& val )
					-> std::enable_if_t<std::is_convertible_v<Name, std::string>, insert_command_t>{
				static function_id_t id{ 0 };
				commands.insert({ id, std::forward( val )});
				cmd_to_id.insert({ mangle( m, std::forward( s ), argc ), id });
				return { this, id++ };
			}

			template<typename T, typename ...Args>
			auto find_and_call( Mode m, T&& name, size_t argc, Args... args )
					-> std::enable_if_t<std::is_convertible_v<T, std::string>, cmd_ret_t>{
				auto t = cmd_to_id.find( mangle( m, std::forward( name ), argc ));
				if( t == cmd_to_id.end())
					return { false, false, "Could not find command :\"" + name + "\"" };

				return ( void (*)( Args... ))( commands.find( t->second )->second())( std::forward<Args>( args )... );
			}

			template<typename T, typename ...Args>
			cmd_ret_t find_and_call( function_id_t id, Args... args ){
				return ( void (*)( Args... ))( commands.find( id )->second())( std::forward<Args>( args )... );
			}

			inline insert_command_t modify( Mode m, const std::string& s, size_t argc ){
				return { this, cmd_to_id.find( mangle( m, s, argc ))->second };
			}

			void alias( function_id_t id, Mode m, std::string a, size_t argc );
			void help( function_id_t id, std::string s );

		private:
			std::map<std::string, function_id_t> cmd_to_id;
			std::map<function_id_t, function_t> commands;
			std::map<function_id_t, std::string> help_text;
	};
}
