#include <gbdk/platform.h>
#include <stdint.h>

// Tile and map data
#include "../res/sprite.h"
#include "../res/dungeon.h"

#include "common.h"


// ====================================================================

void update_player_sprite(uint8_t dir) {

	// Priority flag is set in metasprite so sprite is under BG & Window except for BG Color 0
    // Rotate through frames using system time (could be updated less often)
	move_metasprite(sprite_metasprites[(dir) + ((sys_time >> 4)& 0x01u)], 0, 0, SPR_PLY_X, SPR_PLY_Y);
}


void init_gfx() {
	
	SPRITES_8x16;
	
    // Load background tiles and map
    set_bkg_data(0, dungeon_TILE_COUNT, dungeon_tiles);
    set_bkg_tiles(0, 0, dungeon_WIDTH >> 3, dungeon_HEIGHT >> 3, dungeon_map);
	// All black palette for Background
	BGP_REG = DMG_PALETTE(DMG_BLACK, DMG_BLACK, DMG_BLACK, DMG_BLACK);
	// Align background to a decent starting point (sprite not in a wall)
 	move_bkg(19,0);

    // Load sprite tiles and set on center of screen
    SPRITES_8x16;
    set_sprite_data(0, sprite_TILE_COUNT, sprite_tiles);
    update_player_sprite(PLY_DIR_LEFT);
	// First color is transparent, doesn't matter
	OBP0_REG = DMG_PALETTE(DMG_BLACK, DMG_WHITE, DMG_LITE_GRAY, DMG_BLACK);

    SHOW_BKG;
    HIDE_WIN;
    SHOW_SPRITES;
}



uint8_t const * p_x_end;

void main(void)
{
	init_gfx();

    // Loop endlessly
    while(1) {

        UPDATE_KEYS(); // Read Joypad

        // Move player (via background scroll) if needed
        if (keys & J_LEFT) {
            scroll_bkg(-1,0);
            update_player_sprite(PLY_DIR_LEFT);
        }
        else if (keys & J_RIGHT) {
            scroll_bkg(1,0);
            update_player_sprite(PLY_DIR_RIGHT);
        }
        else if (keys & J_UP) {
            scroll_bkg(0,-1);
            update_player_sprite(PLY_DIR_UP);
        }
        else if (keys & J_DOWN) {
            scroll_bkg(0,1);
            update_player_sprite(PLY_DIR_DOWN);
        }
 
        wait_vbl_done(); // Yield CPU till the end of each frame
    }
}
