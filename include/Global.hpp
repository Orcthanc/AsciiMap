#pragma once

#include "Command.hpp"
#include "Map.hpp"

namespace Pathfinder {
	struct GlobalData {
		public:
			inline static void init(){
				data = {};
			}

			constexpr static GlobalData* get(){
				return &data;
			}

			Commands commands;
			ConsoleRenderer* renderer;
		private:
			static GlobalData data;
	};
}
