#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>

// Tile and map data
#include "sprite.h"
#include "dungeon.h"

#include "common.h"

// Clamps to hardware tilemap width and height in tiles
// Avoids Y step down when wrapping around > 31
#define TM_CLAMP_W(n) (n & ((uint8_t)DEVICE_SCREEN_BUFFER_WIDTH - 1u))
#define TM_CLAMP_H(n) (n & ((uint8_t)DEVICE_SCREEN_BUFFER_HEIGHT - 1u))

// ====================================================================

void update_player_sprite(uint8_t dir) {

	// Priority flag is set in metasprite so sprite is under BG & Window except for BG Color 0
    // Rotate through frames using system time (could be updated less often)
	move_metasprite(sprite_metasprites[(dir) + ((sys_time >> 4)& 0x01u)], 0, 0, SPR_PLY_X, SPR_PLY_Y);
}


#define PAL_DRK   0u
#define PAL_MED   1u
#define PAL_LGT   2u
#define TORCH_WIDTH     7u
#define TORCH_HEIGHT    7u
#define TORCH_WIDTH_ST  3u
#define TORCH_HEIGHT_ST 3u

const uint8_t torch_attrib_overlay_map[TORCH_WIDTH * TORCH_HEIGHT] = {
    // Large more circulat shape
    PAL_DRK, PAL_DRK, PAL_MED, PAL_MED, PAL_MED, PAL_DRK, PAL_DRK,
    PAL_DRK, PAL_MED, PAL_LGT, PAL_LGT, PAL_LGT, PAL_MED, PAL_DRK,
    PAL_MED, PAL_LGT, PAL_LGT, PAL_LGT, PAL_LGT, PAL_LGT, PAL_MED,

    PAL_MED, PAL_LGT, PAL_LGT, PAL_LGT, PAL_LGT, PAL_LGT, PAL_MED,

    PAL_MED, PAL_LGT, PAL_LGT, PAL_LGT, PAL_LGT, PAL_LGT, PAL_MED,
    PAL_DRK, PAL_MED, PAL_LGT, PAL_LGT, PAL_LGT, PAL_MED, PAL_DRK,
    PAL_DRK, PAL_DRK, PAL_MED, PAL_MED, PAL_MED, PAL_DRK, PAL_DRK,

    // Smaller diamond shape
    // PAL_DRK, PAL_DRK, PAL_DRK, PAL_MED, PAL_DRK, PAL_DRK, PAL_DRK,
    // PAL_DRK, PAL_DRK, PAL_MED, PAL_LGT, PAL_MED, PAL_DRK, PAL_DRK,
    // PAL_DRK, PAL_MED, PAL_LGT, PAL_LGT, PAL_LGT, PAL_MED, PAL_DRK,

    // PAL_MED, PAL_LGT, PAL_LGT, PAL_LGT, PAL_LGT, PAL_LGT, PAL_MED,

    // PAL_DRK, PAL_MED, PAL_LGT, PAL_LGT, PAL_LGT, PAL_MED, PAL_DRK,
    // PAL_DRK, PAL_DRK, PAL_MED, PAL_LGT, PAL_MED, PAL_DRK, PAL_DRK,
    // PAL_DRK, PAL_DRK, PAL_DRK, PAL_MED, PAL_DRK, PAL_DRK, PAL_DRK,
};


static uint8_t atr_cent_x_last = 0u;
static uint8_t atr_cent_y_last = 0u;

void redraw_attribute_map_torch_effect(void) {

    uint8_t atr_cent_x = ((SPR_PLY_X - DEVICE_SPRITE_PX_OFFSET_X) + SCX_REG) >> 3;
    uint8_t atr_cent_y = ((SPR_PLY_Y - DEVICE_SPRITE_PX_OFFSET_Y) + SCY_REG) >> 3;

    VBK_REG = VBK_ATTRIBUTES;
    // Draw edges
    set_bkg_tiles(TM_CLAMP_W(atr_cent_x - TORCH_WIDTH_ST),
                  TM_CLAMP_H(atr_cent_y - TORCH_HEIGHT_ST),
                  TORCH_WIDTH, TORCH_HEIGHT, torch_attrib_overlay_map);
    // fill_bkg_rect(atr_cent_x - 1, atr_cent_y - 2, 3, 1, PAL_MED);
    // fill_bkg_rect(atr_cent_x - 1, atr_cent_y + 2, 3, 1, PAL_MED);
    // fill_bkg_rect(atr_cent_x - 2, atr_cent_y - 1, 1, 3, PAL_MED);
    // fill_bkg_rect(atr_cent_x + 2, atr_cent_y - 1, 1, 3, PAL_MED);
    // // Draw Center
    // fill_bkg_rect(atr_cent_x - 1, atr_cent_y - 1, 3, 3, PAL_LIGHT);

    // // Diamond shape on edges lighter
    // set_bkg_tile_xy(atr_cent_x - 2, atr_cent_y,     PAL_LIGHT);
    // set_bkg_tile_xy(atr_cent_x + 2, atr_cent_y,     PAL_LIGHT);
    // set_bkg_tile_xy(atr_cent_x    , atr_cent_y - 2, PAL_LIGHT);
    // set_bkg_tile_xy(atr_cent_x    , atr_cent_y + 2, PAL_LIGHT);
    // // Diamond shape on outer edges darker
    // set_bkg_tile_xy(atr_cent_x - 3, atr_cent_y,     PAL_MED);
    // set_bkg_tile_xy(atr_cent_x + 3, atr_cent_y,     PAL_MED);
    // set_bkg_tile_xy(atr_cent_x    , atr_cent_y - 3, PAL_MED);
    // set_bkg_tile_xy(atr_cent_x    , atr_cent_y + 3, PAL_MED);

    VBK_REG = VBK_TILES;

    atr_cent_x_last = atr_cent_x;
    atr_cent_y_last = atr_cent_y;
}


void update_attribute_map_torch_effect(uint8_t move_dir) {

    uint8_t atr_cent_x = ((SPR_PLY_X - DEVICE_SPRITE_PX_OFFSET_X) + SCX_REG) >> 3;
    uint8_t atr_cent_y = ((SPR_PLY_Y - DEVICE_SPRITE_PX_OFFSET_Y) + SCY_REG) >> 3;

    if ((atr_cent_x != atr_cent_x_last) || (atr_cent_y != atr_cent_y_last)) {

        VBK_REG = VBK_ATTRIBUTES;
        // Erase Edge of old position

        if (move_dir & J_LEFT) {
            fill_bkg_rect(TM_CLAMP_W(atr_cent_x_last + TORCH_WIDTH_ST),
                          TM_CLAMP_H(atr_cent_y_last - TORCH_HEIGHT_ST), 1, TORCH_HEIGHT, PAL_DRK);
        }
        else if (move_dir & J_RIGHT) {
            fill_bkg_rect(TM_CLAMP_W(atr_cent_x_last - TORCH_WIDTH_ST),
                          TM_CLAMP_H(atr_cent_y_last - TORCH_HEIGHT_ST), 1, TORCH_HEIGHT, PAL_DRK);
        }

        if (move_dir & J_UP) {
            fill_bkg_rect(TM_CLAMP_W(atr_cent_x_last - TORCH_WIDTH_ST),
                          TM_CLAMP_H(atr_cent_y_last + TORCH_HEIGHT_ST), TORCH_WIDTH, 1, PAL_DRK);
        }
        else if (move_dir & J_DOWN) {
            fill_bkg_rect(TM_CLAMP_W(atr_cent_x_last - TORCH_WIDTH_ST),
                          TM_CLAMP_H(atr_cent_y_last - TORCH_HEIGHT_ST), TORCH_WIDTH, 1, PAL_DRK);
        }

        // Draw circle of lighter tile attributes around new position
        // using a pre-made small tilemap with light palettes in the center
        set_bkg_tiles(TM_CLAMP_W(atr_cent_x - TORCH_WIDTH_ST),
                      TM_CLAMP_H(atr_cent_y - TORCH_HEIGHT_ST),
                      TORCH_WIDTH, TORCH_HEIGHT, torch_attrib_overlay_map);

        VBK_REG = VBK_TILES;
    }

    atr_cent_x_last = atr_cent_x;
    atr_cent_y_last = atr_cent_y;
}


void init_gfx(void) {

	SPRITES_8x16;
    // Align background to a decent starting point (sprite not in a wall)
	SCX_REG = 115u;
	SCY_REG = 110u;

    // Load background tiles and map
    set_bkg_data(0, dungeon_TILE_COUNT, dungeon_tiles);
    VBK_REG = VBK_ATTRIBUTES;
    set_bkg_tiles(0, 0, dungeon_WIDTH >> 3, dungeon_HEIGHT >> 3, dungeon_map_attributes);
    VBK_REG = VBK_TILES;
    set_bkg_tiles(0, 0, dungeon_WIDTH >> 3, dungeon_HEIGHT >> 3, dungeon_map);

    set_bkg_palette(0, dungeon_PALETTE_COUNT, dungeon_palettes);

    redraw_attribute_map_torch_effect();

    // Load sprite tiles and set on center of screen
    SPRITES_8x16;
    set_sprite_data(0, sprite_TILE_COUNT, sprite_tiles);
    update_player_sprite(PLY_DIR_LEFT);
	set_sprite_palette(0, sprite_PALETTE_COUNT, sprite_palettes);

    SHOW_BKG;
    HIDE_WIN;
    SHOW_SPRITES;
}


void main(void)
{
	init_gfx();

    // Loop endlessly
    while(1) {

        UPDATE_KEYS(); // Read Joypad
        uint8_t move_dir = keys & (J_LEFT | J_RIGHT | J_UP | J_DOWN);

        // Move player (via background scroll) if needed
        if (move_dir & J_LEFT) {
            scroll_bkg(-1,0);
            update_player_sprite(PLY_DIR_LEFT);
        }
        else if (move_dir & J_RIGHT) {
            scroll_bkg(1,0);
            update_player_sprite(PLY_DIR_RIGHT);
        }

        if (move_dir & J_UP) {
            scroll_bkg(0,-1);
            update_player_sprite(PLY_DIR_UP);
        }
        else if (move_dir & J_DOWN) {
            scroll_bkg(0,1);
            update_player_sprite(PLY_DIR_DOWN);
        }

        update_attribute_map_torch_effect(move_dir);
        wait_vbl_done(); // Yield CPU till the end of each frame
    }
}

