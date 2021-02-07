#include <gb/gb.h>
#include "../res/dungeon_map.h"
#include "../res/dungeon_tiles.h"
#include "../res/sprite_tiles.h"


 // Relies on altered CODE location: -Wl-b_CODE=0x0300
// 128 bit aligned
const UINT8 __at(0x200) X_END_LUT_SM[33] = {
 0x57u, 0x5Cu, 0x5Eu, 0x60u, 0x61u, 0x62u, 0x63u, 0x64u, 0x64u, 0x65u, 0x65u, 
 0x66u, 0x66u, 0x66u, 0x66u, 0x66u, 0x67u, 0x66u, 0x66u, 0x66u, 0x66u, 0x66u, 
 0x65u, 0x65u, 0x64u, 0x64u, 0x63u, 0x62u, 0x61u, 0x60u, 0x5Eu, 0x5Cu, 0x57u
 };

// 128 bit aligned
const UINT8 __at(0x280) X_END_LUT_LG[65] = {
 0x57u, 0x5Eu, 0x62u, 0x64u, 0x66u, 0x68u, 0x69u, 0x6Au, 0x6Cu, 0x6Du, 0x6Eu, 
 0x6Fu, 0x6Fu, 0x70u, 0x71u, 0x72u, 0x72u, 0x73u, 0x73u, 0x74u, 0x74u, 0x75u, 
 0x75u, 0x75u, 0x75u, 0x76u, 0x76u, 0x76u, 0x76u, 0x76u, 0x76u, 0x76u, 0x77u, 
 0x76u, 0x76u, 0x76u, 0x76u, 0x76u, 0x76u, 0x76u, 0x75u, 0x75u, 0x75u, 0x75u, 
 0x74u, 0x74u, 0x73u, 0x73u, 0x72u, 0x72u, 0x71u, 0x70u, 0x6Fu, 0x6Fu, 0x6Eu,
 0x6Du, 0x6Cu, 0x6Au, 0x69u, 0x68u, 0x66u, 0x64u, 0x62u, 0x5Eu, 0x57u
 };


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

#define SPR_PLY_X (((160U / 2U) - 8U) + 8U)  // Centered. X position for sprites is -8 from set position
#define SPR_PLY_Y (((144U / 2U) - 8U) + 16U) // Centered. Y Position for sprites is -16 from set position

#define PLY_DIR_LEFT   0U
#define PLY_DIR_UP     2U
#define PLY_DIR_RIGHT  4U
#define PLY_DIR_DOWN   6U
#define PLY_DIR_RESET  0xFF


// Make assumptions about the state of LCDC reg to avoid having to OR/XOR the window bit
//#define SHOW_WIN_FASTER LCDC_REG = 0xE1
//#define HIDE_WIN_FASTER LCDC_REG = 0xC1
#define SHOW_WIN_FASTER LCDC_REG = 0xC0
#define HIDE_WIN_FASTER LCDC_REG = 0xC1

UINT8 hdelay = 2;

void hblank_isr() {
//    // Don't start until line [effect_y_line]
//    if (LY_REG == 48)
//        HIDE_WIN_FASTER;
//    else if (LY_REG == 96)
//        SHOW_WIN_FASTER;
    // Applies after X and into top of new frame
//    if (LY_REG == 48)
//        HIDE_WIN_FASTER;

//    if (LY_REG > 96)
//        SHOW_WIN_FASTER;
//    else 
LCDC_REG = 0xE1;
WX_REG = 7;

//    if (LY_REG < 48)
	if (LY_REG == y_line_end)
    {
        HIDE_WIN_FASTER;
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        SHOW_WIN_FASTER;        
   }
}

// Set LYC = 48
//   Add second handler
//   set LYC = 96
//     repeat loop 

// TAC_REG = 0x01;

// PUSHES 46
// POPS   28

// 80 Clocks from RST -> first instruction

// 214 start typical
// 136 ISR actual code
// 92 end isr code
// 8

void init_isr() {
    // Add the hblank ISR and enable it
    disable_interrupts();
	LYC_REG = 48;  // Start ISR at line 48
	STAT_REG = 0x40;   // LYC ISR = ON
//    STAT_REG = 0x18; // HBlank ISR = ON
	// Use the ISR from a seaprate ASM file instead
//    add_LCD(hblank_isr);
//    add_LCD(nowait_int_handler); // Override default ISR exit behavior, don't wait for mode
    set_interrupts(VBL_IFLAG | LCD_IFLAG);
    enable_interrupts();
}


void update_player_sprite(UINT8 dir) {
    static UINT8 tile_id = PLY_DIR_RESET;

    tile_id = dir * 4U;

    set_sprite_tile(SPR_PLY_LEFT, tile_id + ((sys_time >> 1) & 0x04));
    set_sprite_tile(SPR_PLY_RIGHT, tile_id + 2U + ((sys_time >> 1) & 0x04));
}


void init_gfx() {
    // Load tiles (background + window) and map
    set_bkg_data(0, 79, dungeon_tiles);
    set_bkg_tiles(0, 0, 32, 32, dungeon_mapPLN0);

    // Set Window map to single solid color, move it to upper left and show it
    fill_win_rect(0, 0, 32, 32,BKG_TILE_WHITE);
    move_win(112,0);

    // Load sprite tiles
    SPRITES_8x16;
    set_sprite_data(0, 8 * 4, sprite_tiles); // 8 (16x16) sprite frames x 4 tiles each
    update_player_sprite(PLY_DIR_LEFT);
    // Center player on screen
    move_sprite(SPR_PLY_LEFT,  SPR_PLY_X,     SPR_PLY_Y);
    move_sprite(SPR_PLY_RIGHT, SPR_PLY_X + 8, SPR_PLY_Y);
    // 3= 3(black),2= 1 (l.gray), 1= 0 (white),  0= 2 (d.gray) TRANSP w/ PRIOR
    // Rearrange palette to (d.grey=transp, white, l.grey, black)
    OBP0_REG = (0x03U << 6) | (0x01U << 4) | (0x00U << 2) | (0x02U);


    SHOW_BKG;
    SHOW_WIN;
    SHOW_SPRITES;
}



#define Y_SIZE  16U // +/- center, so 2x for full size
#define Y_GROW  16u
#define Y_START (UINT8)((144U / 2U) - Y_SIZE)
#define Y_END   (UINT8)((144U / 2U) + Y_SIZE) + 1 // +1 for proper centering

#define X_SIZE  16U // +/- center, so 2x for full size
#define X_GROW  16u
#define X_START (UINT8)((160U / 2U) - X_SIZE)
#define X_END   (UINT8)((160U / 2U) + X_SIZE) + 1 // +1 for proper centering

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
			LYC_REG	   = Y_START;
			WX_REG     = X_END;
			y_line_end = Y_END;
            p_x_end = &X_END_LUT_SM[0];
		} else {
			LYC_REG	   = (Y_START - Y_GROW);   // Top   // Can add one here to alternate Y line timing
			WX_REG     = (X_END + X_GROW);     // Right
			y_line_end = (Y_END + Y_GROW);     // Bottom
            p_x_end = &X_END_LUT_LG[0];
		}
        UPDATE_KEYS(); // Read Joypad

// WX_REG = *p_x_end;
 p_x_end++;

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
