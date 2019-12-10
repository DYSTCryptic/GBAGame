
//{{BLOCK(city)

//======================================================================
//
//	city, 256x256@4, 
//	+ palette 256 entries, not compressed
//	+ 351 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 11232 + 2048 = 13792
//
//	Time-stamp: 2019-11-18, 19:46:32
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_CITY_H
#define GRIT_CITY_H

#define cityTilesLen 11232
extern const unsigned short cityTiles[5616];

#define cityMapLen 2048
extern const unsigned short cityMap[1024];

#define cityPalLen 512
extern const unsigned short cityPal[256];

#endif // GRIT_CITY_H

//}}BLOCK(city)
