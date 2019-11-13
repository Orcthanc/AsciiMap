#pragma once

#include <stdlib.h>
#include <stdexcept>

namespace Pathfinder {
	static constexpr size_t name_to_enum( const char* name ){
		size_t i = 0;

		for( int i = 0; i < 4; ++i ){
			i <<= 2;
			switch( *( name++ )){
				case 'n':
					break;
				case 'w':
					i += 1;
					break;
				case 't':
					i += 2;
					break;
				case 'd':
					i += 3;
					break;
				default:
					throw std::runtime_error( "Invalid argument in name_to_size" );
			}
		}

		return i;
	}

	namespace Wall {
		//ll dd uu rr
		//00 = none
		//01 = wall
		//10 = thick wall
		//11 = double wall
#define WALLS			\
						\
WALL( nnnn )			\
WALL( nnnw )			\
WALL( nnnt )			\
WALL( nnnd )			\
						\
WALL( nnwn )			\
WALL( nnww )			\
WALL( nnwt )			\
WALL( nnwd )			\
						\
WALL( nntn )			\
WALL( nntw )			\
WALL( nntt )			\
WALL( nntd )			\
						\
WALL( nndn )			\
WALL( nndw )			\
WALL( nndt )			\
WALL( nndd )			\
						\
						\
WALL( nwnn )			\
WALL( nwnw )			\
WALL( nwnt )			\
WALL( nwnd )			\
						\
WALL( nwwn )			\
WALL( nwww )			\
WALL( nwwt )			\
WALL( nwwd )			\
						\
WALL( nwtn )			\
WALL( nwtw )			\
WALL( nwtt )			\
WALL( nwtd )			\
						\
WALL( nwdn )			\
WALL( nwdw )			\
WALL( nwdt )			\
WALL( nwdd )			\
						\
						\
WALL( ntnn )			\
WALL( ntnw )			\
WALL( ntnt )			\
WALL( ntnd )			\
						\
WALL( ntwn )			\
WALL( ntww )			\
WALL( ntwt )			\
WALL( ntwd )			\
						\
WALL( nttn )			\
WALL( nttw )			\
WALL( nttt )			\
WALL( nttd )			\
						\
WALL( ntdn )			\
WALL( ntdw )			\
WALL( ntdt )			\
WALL( ntdd )			\
						\
						\
WALL( ndnn )			\
WALL( ndnw )			\
WALL( ndnt )			\
WALL( ndnd )			\
						\
WALL( ndwn )			\
WALL( ndww )			\
WALL( ndwt )			\
WALL( ndwd )			\
						\
WALL( ndtn )			\
WALL( ndtw )			\
WALL( ndtt )			\
WALL( ndtd )			\
						\
WALL( nddn )			\
WALL( nddw )			\
WALL( nddt )			\
WALL( nddd )			\
						\
						\
						\
						\
WALL( wnnn )			\
WALL( wnnw )			\
WALL( wnnt )			\
WALL( wnnd )			\
						\
WALL( wnwn )			\
WALL( wnww )			\
WALL( wnwt )			\
WALL( wnwd )			\
						\
WALL( wntn )			\
WALL( wntw )			\
WALL( wntt )			\
WALL( wntd )			\
						\
WALL( wndn )			\
WALL( wndw )			\
WALL( wndt )			\
WALL( wndd )			\
						\
						\
WALL( wwnn )			\
WALL( wwnw )			\
WALL( wwnt )			\
WALL( wwnd )			\
						\
WALL( wwwn )			\
WALL( wwww )			\
WALL( wwwt )			\
WALL( wwwd )			\
						\
WALL( wwtn )			\
WALL( wwtw )			\
WALL( wwtt )			\
WALL( wwtd )			\
						\
WALL( wwdn )			\
WALL( wwdw )			\
WALL( wwdt )			\
WALL( wwdd )			\
						\
						\
WALL( wtnn )			\
WALL( wtnw )			\
WALL( wtnt )			\
WALL( wtnd )			\
						\
WALL( wtwn )			\
WALL( wtww )			\
WALL( wtwt )			\
WALL( wtwd )			\
						\
WALL( wttn )			\
WALL( wttw )			\
WALL( wttt )			\
WALL( wttd )			\
						\
WALL( wtdn )			\
WALL( wtdw )			\
WALL( wtdt )			\
WALL( wtdd )			\
						\
						\
WALL( wdnn )			\
WALL( wdnw )			\
WALL( wdnt )			\
WALL( wdnd )			\
						\
WALL( wdwn )			\
WALL( wdww )			\
WALL( wdwt )			\
WALL( wdwd )			\
						\
WALL( wdtn )			\
WALL( wdtw )			\
WALL( wdtt )			\
WALL( wdtd )			\
						\
WALL( wddn )			\
WALL( wddw )			\
WALL( wddt )			\
WALL( wddd )			\
						\
						\
						\
						\
WALL( tnnn )			\
WALL( tnnw )			\
WALL( tnnt )			\
WALL( tnnd )			\
						\
WALL( tnwn )			\
WALL( tnww )			\
WALL( tnwt )			\
WALL( tnwd )			\
						\
WALL( tntn )			\
WALL( tntw )			\
WALL( tntt )			\
WALL( tntd )			\
						\
WALL( tndn )			\
WALL( tndw )			\
WALL( tndt )			\
WALL( tndd )			\
						\
						\
WALL( twnn )			\
WALL( twnw )			\
WALL( twnt )			\
WALL( twnd )			\
						\
WALL( twwn )			\
WALL( twww )			\
WALL( twwt )			\
WALL( twwd )			\
						\
WALL( twtn )			\
WALL( twtw )			\
WALL( twtt )			\
WALL( twtd )			\
						\
WALL( twdn )			\
WALL( twdw )			\
WALL( twdt )			\
WALL( twdd )			\
						\
						\
WALL( ttnn )			\
WALL( ttnw )			\
WALL( ttnt )			\
WALL( ttnd )			\
						\
WALL( ttwn )			\
WALL( ttww )			\
WALL( ttwt )			\
WALL( ttwd )			\
						\
WALL( tttn )			\
WALL( tttw )			\
WALL( tttt )			\
WALL( tttd )			\
						\
WALL( ttdn )			\
WALL( ttdw )			\
WALL( ttdt )			\
WALL( ttdd )			\
						\
						\
WALL( tdnn )			\
WALL( tdnw )			\
WALL( tdnt )			\
WALL( tdnd )			\
						\
WALL( tdwn )			\
WALL( tdww )			\
WALL( tdwt )			\
WALL( tdwd )			\
						\
WALL( tdtn )			\
WALL( tdtw )			\
WALL( tdtt )			\
WALL( tdtd )			\
						\
WALL( tddn )			\
WALL( tddw )			\
WALL( tddt )			\
WALL( tddd )			\
						\
						\
						\
						\
WALL( dnnn )			\
WALL( dnnw )			\
WALL( dnnt )			\
WALL( dnnd )			\
						\
WALL( dnwn )			\
WALL( dnww )			\
WALL( dnwt )			\
WALL( dnwd )			\
						\
WALL( dntn )			\
WALL( dntw )			\
WALL( dntt )			\
WALL( dntd )			\
						\
WALL( dndn )			\
WALL( dndw )			\
WALL( dndt )			\
WALL( dndd )			\
						\
						\
WALL( dwnn )			\
WALL( dwnw )			\
WALL( dwnt )			\
WALL( dwnd )			\
						\
WALL( dwwn )			\
WALL( dwww )			\
WALL( dwwt )			\
WALL( dwwd )			\
						\
WALL( dwtn )			\
WALL( dwtw )			\
WALL( dwtt )			\
WALL( dwtd )			\
						\
WALL( dwdn )			\
WALL( dwdw )			\
WALL( dwdt )			\
WALL( dwdd )			\
						\
						\
WALL( dtnn )			\
WALL( dtnw )			\
WALL( dtnt )			\
WALL( dtnd )			\
						\
WALL( dtwn )			\
WALL( dtww )			\
WALL( dtwt )			\
WALL( dtwd )			\
						\
WALL( dttn )			\
WALL( dttw )			\
WALL( dttt )			\
WALL( dttd )			\
						\
WALL( dtdn )			\
WALL( dtdw )			\
WALL( dtdt )			\
WALL( dtdd )			\
						\
						\
WALL( ddnn )			\
WALL( ddnw )			\
WALL( ddnt )			\
WALL( ddnd )			\
						\
WALL( ddwn )			\
WALL( ddww )			\
WALL( ddwt )			\
WALL( ddwd )			\
						\
WALL( ddtn )			\
WALL( ddtw )			\
WALL( ddtt )			\
WALL( ddtd )			\
						\
WALL( dddn )			\
WALL( dddw )			\
WALL( dddt )			\
WALL( dddd )


#define WALL( name ) name = name_to_enum( #name ),

		enum Wall {
			WALLS
		};

	}

	const std::string dir_to_chars( Wall::Wall dir );
}
