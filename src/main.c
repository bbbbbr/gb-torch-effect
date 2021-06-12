#include <gb/gb.h>
#include <stdint.h>
#include <string.h>


// Tile and map data
#include "../res/dungeon_map.h"
#include "../res/dungeon_tiles.h"

#include "../res/sprite_tiles.h"

#include "../res/circle_small_tiles.h"
#include "../res/circle_large_tiles.h"

#include "common.h"

// Right edge window mask data
#include "torch_win_data.h"
#include "torch_sprite_data.h"

// TODO: credit notes: tileset from https://sondanielson.itch.io/gameboy-simple-rpg-tileset

// ====================================================================

uint8_t y_line_end = 0;

void init_isr() {
    // Add the hblank ISR and enable it
    disable_interrupts();
	LYC_REG = 48;      // Start ISR at line 48
	STAT_REG = 0x40;   // LYC ISR = ON
	// LCD ISR is in separate ASM file, no need to install it
    set_interrupts(VBL_IFLAG | LCD_IFLAG);
    enable_interrupts();
}


void update_player_sprite(uint8_t dir) {

    static uint8_t tile_id = PLY_DIR_RESET;

    tile_id = dir * 4U;
    // Rotate through frames using system time (could be updated less often)
    set_sprite_tile(SPR_PLY_LEFT, tile_id + ((sys_time >> 1) & 0x04));
    set_sprite_tile(SPR_PLY_RIGHT, tile_id + 2u + ((sys_time >> 1) & 0x04));
}


void init_gfx() {

    // Load tiles (background + window) and map
    set_bkg_data(0, 79, dungeon_tiles);
    set_bkg_tiles(0, 0, 32, 32, dungeon_mapPLN0);
    // Set Window map to single solid color, move it to upper left and show it
    fill_win_rect(0, 0, 32, 32,BKG_TILE_BLACK);
    move_win(112,0);


    // Load sprite tiles
    SPRITES_8x16;
    set_sprite_data(0, SPR_TILE_COUNT_PLAYER, sprite_tiles); // 8 (16x16) sprite frames x 4 tiles each   
    update_player_sprite(PLY_DIR_LEFT);
    // Center player on screen
    move_sprite(SPR_PLY_LEFT,  SPR_PLY_X,     SPR_PLY_Y);
    move_sprite(SPR_PLY_RIGHT, SPR_PLY_X + 8, SPR_PLY_Y);
    // 3= 3(black),2= 1 (l.gray), 1= 0 (white),  0= 2 (d.gray) TRANSP w/ PRIOR
    // Rearrange palette to (d.grey=transp, white, l.grey, black)
    OBP0_REG = (0x03u << 6) | (0x01u << 4) | (0x00u << 2) | (0x02u);

    // Window mask sprite data
    set_sprite_data(SPR_TILE_START_MASK_SM, SPR_TILE_COUNT_MASK_SM, circle_small_tiles);
    set_sprite_data(SPR_TILE_START_MASK_LG, SPR_TILE_COUNT_MASK_LG, circle_large_tiles);

    // Copy sprites that don't change into Shadow OAM  
    memcpy( &(shadow_OAM[spr_count_fixed_start]),  // dest (offset into shadow OAM)
            &(spr_circle_lg_fixed[0]),             // src (sprite data in OAM format)
            spr_circle_lg_fixed_size);

    SHOW_BKG;
    SHOW_WIN;
    SHOW_SPRITES;
}



uint8_t const * p_x_end;

void main(void)
{
    // Pointer to one of the Window circle LUTs
    p_x_end = &X_END_LUT_SM[0];

	init_gfx();

    // Try to align with the start of a frame as in the loop below
    wait_vbl_done();
    init_isr();

    // Loop endlessly
    while(1) {

        // Alternate windowed size
		if (sys_time & 0x01) {
            // SMALL circle window config            
			LYC_REG	   = Y_START;            // Top
			WX_REG     = X_END;              // Right
			y_line_end = Y_END;              // Bottom
            p_x_end = &X_END_LUT_SM[0];

            // Left Edge masking
            // Set up *LARGE* circle to copy to OAM on next vblank (via shadow oam)
            memcpy( &(shadow_OAM[spr_count_change_start]), // dest (offset into shadow OAM)
                    &(spr_circle_lg[0]),                   // src (sprite data in OAM format)
                    spr_circle_lg_size);
		} else {
            // LARGE circle window config
			LYC_REG	   = (Y_START - Y_GROW);   // Top   // Can add one here to alternate Y line timing
			WX_REG     = (X_END + X_GROW);     // Right
			y_line_end = (Y_END + Y_GROW);     // Bottom
            p_x_end = &X_END_LUT_LG[0];

            // Left Edge masking
            // Set up *SMALL* circle to copy to OAM on next vblank (via shadow oam)
            memcpy( &(shadow_OAM[spr_count_change_start]), // dest (offset into shadow OAM)
                    &(spr_circle_sm[0]),                   // src (sprite data in OAM format)
                    spr_circle_sm_size);
		}

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
