#include <gb/gb.h>
#include "res/dungeon_map.h"
#include "res/dungeon_tiles.h"


UINT8 keys;
UINT8 previous_keys;

#define UPDATE_KEYS() previous_keys = keys; keys = joypad()
#define KEY_PRESSED(K) (keys & (K))
#define KEY_TICKED(K)   ((keys & (K)) && !(previous_keys & (K)))
#define KEY_RELEASED(K) ((previous_keys & (K)) && !(keys & (K)))
#define ANY_KEY_PRESSED (keys)


void init_gfx() {
    // Load tiles (background + window)
    set_bkg_data(0, 79, dungeon_tiles);

    // Load background map
    set_bkg_tiles(0, 0, 32, 32, dungeon_mapPLN0);
    SHOW_BKG;

//    WX_REG = win_x;
//    WY_REG = win_y;
//    SHOW_WIN;
}

void main(void)
{
	init_gfx();

    // Loop endlessly
    while(1) {

        UPDATE_KEYS(); // Read Joypad

        if      (keys & J_LEFT) scroll_bkg(-1,0);
        else if (keys & J_RIGHT) scroll_bkg(1,0);

        if      (keys & J_UP)   scroll_bkg(0,-1);
        else if (keys & J_DOWN) scroll_bkg(0,1);
 
        wait_vbl_done(); // Yield CPU till the end of each frame
    }
}
