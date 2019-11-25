#pragma once

#include <map>

namespace Pathfinder {
	struct Commands;
		
	using void_function_t = void (*)( void );
	using function_id_t = size_t;

	struct insert_command_t {
		insert_command_t& alias( std::string s );
		insert_command_t& help( std::string s );

		Commands* cmds;
		function_id_t cmd;
	};

	struct Commands {

		public:
			template<typename T>
			insert_command_t insert( std::string s, T&& val ){
				static function_id_t id{ 0 };
				commands.insert({ id, std::forward( val )});
				cmd_to_id.insert({ s, id });
				return { this, id++ };
			}

			template<typename T, typename ...Args>
			std::enable_if_t<std::is_convertible_v<T, std::string>, bool> find_and_call( const T&& name, Args... args ){
				auto t = cmd_to_id.find( std::forward( name ));
				if( t == cmd_to_id.end())
					return false;

				( void (*)( Args... ))( commands.find( t->second )->second())( std::forward<Args>( args )... );

				return true;
			}

			//TODO google
			template<typename T, typename ...Args>
			bool find_and_call( function_id_t id, Args... args ){
				( void (*)( Args... ))( commands.find( id )->second())( std::forward<Args>( args )... );
				return true;
			}

			inline insert_command_t modify( const std::string& s ){
				return { this, cmd_to_id.find( s )->second };
			}

			void alias( function_id_t id, std::string a );
			void help( function_id_t id, std::string s );

		private:
			std::map<std::string, function_id_t> cmd_to_id;
			std::map<function_id_t, void_function_t> commands;
			std::map<function_id_t, std::string> help_text;
	};
}
