#include <iostream>

#include "Global.hpp"
#include "Map.hpp"
#include "ConsoleRenderer.hpp"

using namespace Pathfinder;

int main( int argc, char** argv ){
	/*
	MapLayer test{
		{
			{
				1,
				{
					1,
					1,
					0,
				}
			},
			{
				2,
				{
					1,
					2,
					0,
				}
			},
			{
				0,
				{
					0,
					1,
					0,
				}
			},
			{
				3,
				{
					2,
					1,
					0,
				}
			},
			{
				3,
				{
					1,
					0,
					0,
				}
			},
			{
				0,
				{
					0,
					1,
					0,
				}
			},
			{
				0,
				{
					1,
					0,
					0,
				}
			},
			{
				0,
				{
					1,
					0,
					0,
				}
			},
			{
				0,
				{
					0,
					0,
					0,
				}
			}
		}, 3, 3 };
*/
	GlobalData::init();

	ConsoleRenderer r{ MapLayer{} };

	r();
}
