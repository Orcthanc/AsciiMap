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
#define DIRECTIONS			\
							\
DIRECTION( nnnn )			\
DIRECTION( nnnw )			\
DIRECTION( nnnt )			\
DIRECTION( nnnd )			\
							\
DIRECTION( nnwn )			\
DIRECTION( nnww )			\
DIRECTION( nnwt )			\
DIRECTION( nnwd )			\
							\
DIRECTION( nntn )			\
DIRECTION( nntw )			\
DIRECTION( nntt )			\
DIRECTION( nntd )			\
							\
DIRECTION( nndn )			\
DIRECTION( nndw )			\
DIRECTION( nndt )			\
DIRECTION( nndd )			\
							\
							\
DIRECTION( nwnn )			\
DIRECTION( nwnw )			\
DIRECTION( nwnt )			\
DIRECTION( nwnd )			\
							\
DIRECTION( nwwn )			\
DIRECTION( nwww )			\
DIRECTION( nwwt )			\
DIRECTION( nwwd )			\
							\
DIRECTION( nwtn )			\
DIRECTION( nwtw )			\
DIRECTION( nwtt )			\
DIRECTION( nwtd )			\
							\
DIRECTION( nwdn )			\
DIRECTION( nwdw )			\
DIRECTION( nwdt )			\
DIRECTION( nwdd )			\
							\
							\
DIRECTION( ntnn )			\
DIRECTION( ntnw )			\
DIRECTION( ntnt )			\
DIRECTION( ntnd )			\
							\
DIRECTION( ntwn )			\
DIRECTION( ntww )			\
DIRECTION( ntwt )			\
DIRECTION( ntwd )			\
							\
DIRECTION( nttn )			\
DIRECTION( nttw )			\
DIRECTION( nttt )			\
DIRECTION( nttd )			\
							\
DIRECTION( ntdn )			\
DIRECTION( ntdw )			\
DIRECTION( ntdt )			\
DIRECTION( ntdd )			\
							\
							\
DIRECTION( ndnn )			\
DIRECTION( ndnw )			\
DIRECTION( ndnt )			\
DIRECTION( ndnd )			\
							\
DIRECTION( ndwn )			\
DIRECTION( ndww )			\
DIRECTION( ndwt )			\
DIRECTION( ndwd )			\
							\
DIRECTION( ndtn )			\
DIRECTION( ndtw )			\
DIRECTION( ndtt )			\
DIRECTION( ndtd )			\
							\
DIRECTION( nddn )			\
DIRECTION( nddw )			\
DIRECTION( nddt )			\
DIRECTION( nddd )			\
							\
							\
							\
							\
DIRECTION( wnnn )			\
DIRECTION( wnnw )			\
DIRECTION( wnnt )			\
DIRECTION( wnnd )			\
							\
DIRECTION( wnwn )			\
DIRECTION( wnww )			\
DIRECTION( wnwt )			\
DIRECTION( wnwd )			\
							\
DIRECTION( wntn )			\
DIRECTION( wntw )			\
DIRECTION( wntt )			\
DIRECTION( wntd )			\
							\
DIRECTION( wndn )			\
DIRECTION( wndw )			\
DIRECTION( wndt )			\
DIRECTION( wndd )			\
							\
							\
DIRECTION( wwnn )			\
DIRECTION( wwnw )			\
DIRECTION( wwnt )			\
DIRECTION( wwnd )			\
							\
DIRECTION( wwwn )			\
DIRECTION( wwww )			\
DIRECTION( wwwt )			\
DIRECTION( wwwd )			\
							\
DIRECTION( wwtn )			\
DIRECTION( wwtw )			\
DIRECTION( wwtt )			\
DIRECTION( wwtd )			\
							\
DIRECTION( wwdn )			\
DIRECTION( wwdw )			\
DIRECTION( wwdt )			\
DIRECTION( wwdd )			\
							\
							\
DIRECTION( wtnn )			\
DIRECTION( wtnw )			\
DIRECTION( wtnt )			\
DIRECTION( wtnd )			\
							\
DIRECTION( wtwn )			\
DIRECTION( wtww )			\
DIRECTION( wtwt )			\
DIRECTION( wtwd )			\
							\
DIRECTION( wttn )			\
DIRECTION( wttw )			\
DIRECTION( wttt )			\
DIRECTION( wttd )			\
							\
DIRECTION( wtdn )			\
DIRECTION( wtdw )			\
DIRECTION( wtdt )			\
DIRECTION( wtdd )			\
							\
							\
DIRECTION( wdnn )			\
DIRECTION( wdnw )			\
DIRECTION( wdnt )			\
DIRECTION( wdnd )			\
							\
DIRECTION( wdwn )			\
DIRECTION( wdww )			\
DIRECTION( wdwt )			\
DIRECTION( wdwd )			\
							\
DIRECTION( wdtn )			\
DIRECTION( wdtw )			\
DIRECTION( wdtt )			\
DIRECTION( wdtd )			\
							\
DIRECTION( wddn )			\
DIRECTION( wddw )			\
DIRECTION( wddt )			\
DIRECTION( wddd )			\
							\
							\
							\
							\
DIRECTION( tnnn )			\
DIRECTION( tnnw )			\
DIRECTION( tnnt )			\
DIRECTION( tnnd )			\
							\
DIRECTION( tnwn )			\
DIRECTION( tnww )			\
DIRECTION( tnwt )			\
DIRECTION( tnwd )			\
							\
DIRECTION( tntn )			\
DIRECTION( tntw )			\
DIRECTION( tntt )			\
DIRECTION( tntd )			\
							\
DIRECTION( tndn )			\
DIRECTION( tndw )			\
DIRECTION( tndt )			\
DIRECTION( tndd )			\
							\
							\
DIRECTION( twnn )			\
DIRECTION( twnw )			\
DIRECTION( twnt )			\
DIRECTION( twnd )			\
							\
DIRECTION( twwn )			\
DIRECTION( twww )			\
DIRECTION( twwt )			\
DIRECTION( twwd )			\
							\
DIRECTION( twtn )			\
DIRECTION( twtw )			\
DIRECTION( twtt )			\
DIRECTION( twtd )			\
							\
DIRECTION( twdn )			\
DIRECTION( twdw )			\
DIRECTION( twdt )			\
DIRECTION( twdd )			\
							\
							\
DIRECTION( ttnn )			\
DIRECTION( ttnw )			\
DIRECTION( ttnt )			\
DIRECTION( ttnd )			\
							\
DIRECTION( ttwn )			\
DIRECTION( ttww )			\
DIRECTION( ttwt )			\
DIRECTION( ttwd )			\
							\
DIRECTION( tttn )			\
DIRECTION( tttw )			\
DIRECTION( tttt )			\
DIRECTION( tttd )			\
							\
DIRECTION( ttdn )			\
DIRECTION( ttdw )			\
DIRECTION( ttdt )			\
DIRECTION( ttdd )			\
							\
							\
DIRECTION( tdnn )			\
DIRECTION( tdnw )			\
DIRECTION( tdnt )			\
DIRECTION( tdnd )			\
							\
DIRECTION( tdwn )			\
DIRECTION( tdww )			\
DIRECTION( tdwt )			\
DIRECTION( tdwd )			\
							\
DIRECTION( tdtn )			\
DIRECTION( tdtw )			\
DIRECTION( tdtt )			\
DIRECTION( tdtd )			\
							\
DIRECTION( tddn )			\
DIRECTION( tddw )			\
DIRECTION( tddt )			\
DIRECTION( tddd )			\
							\
							\
							\
							\
DIRECTION( dnnn )			\
DIRECTION( dnnw )			\
DIRECTION( dnnt )			\
DIRECTION( dnnd )			\
							\
DIRECTION( dnwn )			\
DIRECTION( dnww )			\
DIRECTION( dnwt )			\
DIRECTION( dnwd )			\
							\
DIRECTION( dntn )			\
DIRECTION( dntw )			\
DIRECTION( dntt )			\
DIRECTION( dntd )			\
							\
DIRECTION( dndn )			\
DIRECTION( dndw )			\
DIRECTION( dndt )			\
DIRECTION( dndd )			\
							\
							\
DIRECTION( dwnn )			\
DIRECTION( dwnw )			\
DIRECTION( dwnt )			\
DIRECTION( dwnd )			\
							\
DIRECTION( dwwn )			\
DIRECTION( dwww )			\
DIRECTION( dwwt )			\
DIRECTION( dwwd )			\
							\
DIRECTION( dwtn )			\
DIRECTION( dwtw )			\
DIRECTION( dwtt )			\
DIRECTION( dwtd )			\
							\
DIRECTION( dwdn )			\
DIRECTION( dwdw )			\
DIRECTION( dwdt )			\
DIRECTION( dwdd )			\
							\
							\
DIRECTION( dtnn )			\
DIRECTION( dtnw )			\
DIRECTION( dtnt )			\
DIRECTION( dtnd )			\
							\
DIRECTION( dtwn )			\
DIRECTION( dtww )			\
DIRECTION( dtwt )			\
DIRECTION( dtwd )			\
							\
DIRECTION( dttn )			\
DIRECTION( dttw )			\
DIRECTION( dttt )			\
DIRECTION( dttd )			\
							\
DIRECTION( dtdn )			\
DIRECTION( dtdw )			\
DIRECTION( dtdt )			\
DIRECTION( dtdd )			\
							\
							\
DIRECTION( ddnn )			\
DIRECTION( ddnw )			\
DIRECTION( ddnt )			\
DIRECTION( ddnd )			\
							\
DIRECTION( ddwn )			\
DIRECTION( ddww )			\
DIRECTION( ddwt )			\
DIRECTION( ddwd )			\
							\
DIRECTION( ddtn )			\
DIRECTION( ddtw )			\
DIRECTION( ddtt )			\
DIRECTION( ddtd )			\
							\
DIRECTION( dddn )			\
DIRECTION( dddw )			\
DIRECTION( dddt )			\
DIRECTION( dddd )


#define DIRECTION( name ) name = name_to_enum( #name ),

		enum Wall {
			DIRECTIONS
		};

	}

	const std::string dir_to_chars( Wall::Wall dir );
}
