#include <gb/gb.h>
#include <stdint.h>
#include <string.h>

#include "../res/dungeon_map.h"
#include "../res/dungeon_tiles.h"

#include "../res/sprite_tiles.h"

#include "../res/circle_small_tiles.h"
#include "../res/circle_large_tiles.h"

// Window boxing and ISR config


#define Y_SIZE  16u // +/- center, so 2x for full size
#define Y_GROW  16u
#define Y_START (UINT8)((144u / 2u) - Y_SIZE)
#define Y_END   (UINT8)((144u / 2u) + Y_SIZE) // + 1 // +1 for proper centering <- not anymore

#define X_SIZE  16u // +/- center, so 2x for full size
#define X_GROW  16u
#define X_START (UINT8)((160u / 2u) - X_SIZE)
#define X_END   (UINT8)((160u / 2u) + X_SIZE) // + 1 // +1 for proper centering <- not anymore


// ====================================================================

// Window Circle Right Edge LUT

 // Relies on altered CODE location: -Wl-b_CODE=0x0300
// 128 bit aligned
const UINT8 __at(0x200) X_END_LUT_SM[33] = {
 0x57u - 8u, 0x5Cu, 0x5Eu, 0x60u, 0x61u, 0x62u, 0x63u, 0x64u, 0x64u, 0x65u, 0x65u, 
 0x66u, 0x66u, 0x66u, 0x66u, 0x66u, 
 0x66u, // mid-point
 0x66u, 0x66u, 0x66u, 0x66u, 0x66u, 
 0x65u, 0x65u, 0x64u, 0x64u, 0x63u, 0x62u, 0x61u, 0x60u, 0x5Eu, 0x5Cu, 0x57u
 };

// 128 bit aligned
const UINT8 __at(0x280) X_END_LUT_LG[65] = {
 0x57u - 8u, 0x5Eu, 0x62u, 0x64u, 0x66u, 0x68u, 0x69u, 0x6Au, 0x6Cu, 0x6Du, 0x6Eu, 
 0x6Fu, 0x6Fu, 0x70u, 0x71u, 0x72u, 0x72u, 0x73u, 0x73u, 0x74u, 0x74u, 0x75u, 
 0x75u, 0x75u, 0x75u, 0x76u, 0x76u, 0x76u, 0x76u, 0x76u, 0x76u, 0x76u, 
 0x76u, // mid-point
 0x76u, 0x76u, 0x76u, 0x76u, 0x76u, 0x76u, 0x76u, 0x75u, 0x75u, 0x75u, 0x75u, 
 0x74u, 0x74u, 0x73u, 0x73u, 0x72u, 0x72u, 0x71u, 0x70u, 0x6Fu, 0x6Fu, 0x6Eu,
 0x6Du, 0x6Cu, 0x6Au, 0x69u, 0x68u, 0x66u, 0x64u, 0x62u, 0x5Eu, 0x57u
 };


// ====================================================================

// Sprite Circle Left edge

 // 8 x 16 sprite mode
#define SPR_TILE_COUNT_PLAYER   (8u * 4u) // 8 (16x16) sprite frames x 4 tiles each (2 frames per direction)
#define SPR_TILE_COUNT_MASK_SM  (4u * 2u) // 3 x (8x16) sprite frames x 2 tiles each
#define SPR_TILE_COUNT_MASK_LG  (6u * 2u) // 5 x (8x16) sprite frames x 2 tiles each
#define SPR_TILE_START_MASK_SM  SPR_TILE_COUNT_PLAYER
#define SPR_TILE_START_MASK_LG  (SPR_TILE_COUNT_PLAYER + SPR_TILE_COUNT_MASK_SM)

#define FLIP_NO 0x00U
#define FLIP_V  0x40U
#define SPR_MASK_ST_X (5 * 8) // Starts 5 tiles from Left edge
#define SPR_MASK_ST_Y ((3.5 * 16) + 1)// Starts 3.5 tiles from Top edge + a small fudge factor
#define C_S(x,y,id,vflip) (y * 16u) + SPR_MASK_ST_Y, (x * 8u) + SPR_MASK_ST_X, (id * 2u) + SPR_TILE_START_MASK_SM, vflip
#define C_L(x,y,id,vflip) (y * 16u) + SPR_MASK_ST_Y, (x * 8u) + SPR_MASK_ST_X, (id * 2u) + SPR_TILE_START_MASK_LG, vflip
#define C_HIDE()          0,0,0,0


// Block of sprites that do NOT change (large circle)
const uint8_t spr_circle_lg_fixed[] = {
    // Row 0
    //  x   y   id  flip
    C_L(0U, 0U, 0U, FLIP_NO),
    C_L(1U, 0U, 0U, FLIP_NO),
    C_L(2U, 0U, 1U, FLIP_NO),
    C_L(3U, 0U, 2U, FLIP_NO),
    C_L(4U, 0U, 3U, FLIP_NO),
    C_L(5U, 0U, 4U, FLIP_NO),

    // Row 1
    C_L(0U, 1U, 0U, FLIP_NO),
    C_L(1U, 1U, 0U, FLIP_NO),

    // Row 2
    C_L(0U, 2U, 0U, FLIP_NO),
    C_L(1U, 2U, 0U, FLIP_NO),

    // Row 3
    //  x   y   id  flip
    C_L(0U, 3U, 0U, FLIP_V),
    C_L(1U, 3U, 0U, FLIP_V),
    C_L(2U, 3U, 1U, FLIP_V),
    C_L(3U, 3U, 2U, FLIP_V),
    C_L(4U, 3U, 3U, FLIP_V),
    C_L(5U, 3U, 4U, FLIP_V)
};


// Center block of sprites that do change
const uint8_t spr_circle_sm[] = {
    // Row 1
    //  x   y   id  flip
    C_S(2U, 1U, 0U, FLIP_NO),
    C_S(3U, 1U, 0U, FLIP_NO),
    C_S(4U, 1U, 1U, FLIP_NO),
    C_S(5U, 1U, 2U, FLIP_NO),
    // Row 2
    C_S(2U, 2U, 0U, FLIP_NO),
    C_S(3U, 2U, 0U, FLIP_NO),
    C_S(4U, 2U, 1U, FLIP_V),
    C_S(5U, 2U, 2U, FLIP_V)
};

// Center block of sprites that do change
const uint8_t spr_circle_lg[] = {
    // Row 1
    //  x   y   id  flip
    C_L(2U, 1U, 5U, FLIP_NO),
    C_HIDE(),
    C_HIDE(),
    C_HIDE(),
    // Row 2
    C_L(2U, 2U, 5U, FLIP_V),
    C_HIDE(),
    C_HIDE(),
    C_HIDE()
};

#define OAM_BYTES_PER_SPRITE  4U
#define SPR_COUNT_LG_FIXED (sizeof(spr_circle_lg_fixed) / OAM_BYTES_PER_SPRITE)
#define SPR_COUNT_LG       (sizeof(spr_circle_lg) / OAM_BYTES_PER_SPRITE)
#define SPR_COUNT_SM       (sizeof(spr_circle_sm) / OAM_BYTES_PER_SPRITE)

#define SPR_COUNT_FIXED_START  (40u - SPR_COUNT_LG_FIXED - SPR_COUNT_LG)  // Align with end of OAM
#define SPR_COUNT_CHANGE_START (SPR_COUNT_FIXED_START + (SPR_COUNT_LG_FIXED))


// ====================================================================



UINT8 keys = 0;
UINT8 previous_keys = 0;

UINT8 y_line_end = 0;

#define UPDATE_KEYS() previous_keys = keys; keys = joypad()
#define KEY_PRESSED(K) (keys & (K))
#define KEY_TICKED(K)   ((keys & (K)) && !(previous_keys & (K)))
#define KEY_RELEASED(K) ((previous_keys & (K)) && !(keys & (K)))
#define ANY_KEY_PRESSED (keys)

#define BKG_TILE_WHITE 1 // 2rd tile in set is solid white
#define BKG_TILE_BLACK 2 // 3rd tile in set is solid black

#define SPR_PLY_LEFT  0
#define SPR_PLY_RIGHT 1

#define SPR_PLY_X (((160u / 2u) - 8u) + 8u)  // Centered. X position for sprites is -8 from set position
#define SPR_PLY_Y (((144u / 2u) - 8u) + 16u) // Centered. Y Position for sprites is -16 from set position

#define PLY_DIR_LEFT   0U
#define PLY_DIR_UP     2U
#define PLY_DIR_RIGHT  4U
#define PLY_DIR_DOWN   6U
#define PLY_DIR_RESET  0xFF


UINT8 hdelay = 2;

void init_isr() {
    // Add the hblank ISR and enable it
    disable_interrupts();
	LYC_REG = 48;      // Start ISR at line 48
	STAT_REG = 0x40;   // LYC ISR = ON
	// LCD ISR is in separate ASM file
    set_interrupts(VBL_IFLAG | LCD_IFLAG);
    enable_interrupts();
}


void update_player_sprite(UINT8 dir) {
    static UINT8 tile_id = PLY_DIR_RESET;

    tile_id = dir * 4U;
    set_sprite_tile(SPR_PLY_LEFT, tile_id + ((sys_time >> 1) & 0x04));
    set_sprite_tile(SPR_PLY_RIGHT, tile_id + 2u + ((sys_time >> 1) & 0x04));
}


void init_gfx() {
    uint8_t c;
    uint8_t * p_shadow_oam;
    uint8_t * p_spr_data;

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

    set_sprite_data(SPR_TILE_START_MASK_SM, SPR_TILE_COUNT_MASK_SM, circle_small_tiles);
    set_sprite_data(SPR_TILE_START_MASK_LG, SPR_TILE_COUNT_MASK_LG, circle_large_tiles);


    // Copy sprites that don't change    
    memcpy( &(shadow_OAM[SPR_COUNT_FIXED_START]), // dest (offset into shadow OAM)
            &(spr_circle_lg_fixed[0]),             // src (sprite data in OAM format)
            sizeof(spr_circle_lg_fixed) );

    SHOW_BKG;
    SHOW_WIN;
    SHOW_SPRITES;
}



UINT8 const * p_x_end;

void main(void)
{
    p_x_end = &X_END_LUT_SM[0];

	init_gfx();
    init_isr();

    // Loop endlessly
    while(1) {

        // Alternate windowed size
		if (sys_time & 0x01) {
            // SMALL circle window config            
			LYC_REG	   = Y_START;
			WX_REG     = X_END;
			y_line_end = Y_END;            
            p_x_end = &X_END_LUT_SM[0];

            // Set up *LARGE* circle to copy to OAM on next vblank
            memcpy( &(shadow_OAM[SPR_COUNT_CHANGE_START]), // dest (offset into shadow OAM)
                    &(spr_circle_lg[0]),                   // src (sprite data in OAM format)
                    sizeof(spr_circle_lg) );
		} else {
			LYC_REG	   = (Y_START - Y_GROW);   // Top   // Can add one here to alternate Y line timing
			WX_REG     = (X_END + X_GROW);     // Right
			y_line_end = (Y_END + Y_GROW);     // Bottom
            p_x_end = &X_END_LUT_LG[0];

            // Set up *SMALL* circle to copy to OAM on next vblank (via shadow oam)
            memcpy( &(shadow_OAM[SPR_COUNT_CHANGE_START]), // dest (offset into shadow OAM)
                    &(spr_circle_sm[0]),                   // src (sprite data in OAM format)
                    sizeof(spr_circle_sm) );            
		}
        UPDATE_KEYS(); // Read Joypad

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
