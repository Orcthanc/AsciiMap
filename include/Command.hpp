#pragma once

#include <map>
#include "ConsoleRenderer.hpp"

namespace Pathfinder {
	struct Commands;

	using function_id_t = size_t;

	template<typename T>
	inline auto mangle( Mode m, T&& name ) -> std::enable_if_t<std::is_convertible_v<T, std::string>, std::string>{
		return std::to_string( static_cast<std::underlying_type_t<Mode>>( m )) + ( std::string )name;
	}

	struct insert_command_t {
		insert_command_t& alias( Mode m, std::string s );
		insert_command_t& help( std::string s );

		Commands* cmds;
		function_id_t cmd;
	};

	struct cmd_ret_t {
		bool success, redraw;
		const std::string message;	//Displayed in lower left
	};

	using function_t = cmd_ret_t (*)( std::vector<const char*> ptr );


	struct Commands {

		public:
			template<typename T, typename Name>
			auto insert( Mode m, Name&& s, T&& val )
					-> std::enable_if_t<std::is_convertible_v<Name, std::string>, insert_command_t>{
				static function_id_t id{ 0 };
				commands.insert({ id, std::forward<T>( val )});
				cmd_to_id.insert({ mangle( m, std::forward<Name>( s )), id });
				return { this, id++ };
			}

			template<typename T, typename ...Args>
			auto find_and_call( Mode m, T&& name, std::vector<const char*> v )
					-> std::enable_if_t<std::is_convertible_v<T, std::string>, cmd_ret_t>{
				auto t = cmd_to_id.find( mangle( m, std::forward<T>( name )));
				if( t == cmd_to_id.end())
					return { false, false, "Could not find command: \"" + std::string( name ) + "\"" };

				return commands.find( t->second )->second( v );
			}

			template<typename T>
			cmd_ret_t find_and_call( function_id_t id, std::vector<const char*> v ){
				return commands.find( id )->second( v );
			}

			inline insert_command_t modify( Mode m, const std::string& s ){
				return { this, cmd_to_id.find( mangle( m, s ))->second };
			}

			void alias( function_id_t id, Mode m, std::string a );
			void help( function_id_t id, std::string s );

		private:
			std::map<std::string, function_id_t> cmd_to_id;
			std::map<function_id_t, function_t> commands;
			std::map<function_id_t, std::string> help_text;
	};
}
