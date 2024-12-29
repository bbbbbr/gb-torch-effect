//AUTOGENERATED FILE FROM png2asset
#ifndef METASPRITE_dungeon_H
#define METASPRITE_dungeon_H

#include <stdint.h>
#include <gbdk/platform.h>
#include <gbdk/metasprites.h>

#define dungeon_TILE_ORIGIN 0
#define dungeon_TILE_W 8
#define dungeon_TILE_H 8
#define dungeon_WIDTH 256
#define dungeon_HEIGHT 256
#define dungeon_TILE_COUNT 71
#define dungeon_PALETTE_COUNT 1
#define dungeon_COLORS_PER_PALETTE 4
#define dungeon_TOTAL_COLORS 4
#define dungeon_MAP_ATTRIBUTES 0

BANKREF_EXTERN(dungeon)

extern const palette_color_t dungeon_palettes[4];
extern const uint8_t dungeon_tiles[1136];

extern const unsigned char dungeon_map[1024];
#define dungeon_map_attributes dungeon_map

#endif