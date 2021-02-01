#include <gb/gb.h>
#include "../res/dungeon_map.h"
#include "../res/dungeon_tiles.h"


UINT8 keys;
UINT8 previous_keys;

UINT8 y_line_end;

#define UPDATE_KEYS() previous_keys = keys; keys = joypad()
#define KEY_PRESSED(K) (keys & (K))
#define KEY_TICKED(K)   ((keys & (K)) && !(previous_keys & (K)))
#define KEY_RELEASED(K) ((previous_keys & (K)) && !(keys & (K)))
#define ANY_KEY_PRESSED (keys)

#define BKG_TILE_WHITE 1 // 2rd tile in set is solid white
#define BKG_TILE_BLACK 2 // 3rd tile in set is solid black

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

void init_gfx() {
    // Load tiles (background + window)
    set_bkg_data(0, 79, dungeon_tiles);

    // Load background map
    set_bkg_tiles(0, 0, 32, 32, dungeon_mapPLN0);
    SHOW_BKG;

//    // Set Window map to solid black, move it to upper left and show it
    fill_win_rect(0,0,31,31,BKG_TILE_WHITE);
    move_win(112,0);
    SHOW_WIN;
}

#define Y_SIZE  16U // +/- center, so 2x for full size
#define Y_GROW  16u
#define Y_START (UINT8)((144U / 2U) - Y_SIZE)
#define Y_END   (UINT8)((144U / 2U) + Y_SIZE)

#define X_SIZE  16U // +/- center, so 2x for full size
#define X_GROW  16u
#define X_START (UINT8)((160U / 2U) - X_SIZE)
#define X_END   (UINT8)((160U / 2U) + X_SIZE)

void main(void)
{
	init_gfx();
    init_isr();

    // Loop endlessly
    while(1) {

		if (sys_time & 0x01) {
			LYC_REG	   = Y_START;
			WX_REG     = X_END;
			y_line_end = Y_END;
		} else {
			LYC_REG	   = (Y_START - Y_GROW);   // Top   // Can add one here to alter Y line timing
			WX_REG     = (X_END + X_GROW);      // Right
			y_line_end = (Y_END + Y_GROW);  // Bottom
		}
        UPDATE_KEYS(); // Read Joypad

        if      (keys & J_LEFT) scroll_bkg(-1,0);
        else if (keys & J_RIGHT) scroll_bkg(1,0);

        if      (keys & J_UP)   scroll_bkg(0,-1);
        else if (keys & J_DOWN) scroll_bkg(0,1);
 
        wait_vbl_done(); // Yield CPU till the end of each frame
    }
}
