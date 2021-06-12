

#ifndef _COMMON_H
#define _COMMON_H
// Sprite Tile Data allocation

 // 8 x 16 sprite mode
#define SPR_TILE_COUNT_PLAYER   (8u * 4u) // 8 (16x16) sprite frames x 4 tiles each (2 frames per direction)
#define SPR_TILE_COUNT_MASK_SM  (4u * 2u) // 3 x (8x16) sprite frames x 2 tiles each
#define SPR_TILE_COUNT_MASK_LG  (6u * 2u) // 5 x (8x16) sprite frames x 2 tiles each
#define SPR_TILE_START_MASK_SM  SPR_TILE_COUNT_PLAYER
#define SPR_TILE_START_MASK_LG  (SPR_TILE_COUNT_PLAYER + SPR_TILE_COUNT_MASK_SM)

#define UPDATE_KEYS() previous_keys = keys; keys = joypad()
#define KEY_PRESSED(K) (keys & (K))
#define KEY_TICKED(K)   ((keys & (K)) && !(previous_keys & (K)))
#define KEY_RELEASED(K) ((previous_keys & (K)) && !(keys & (K)))
#define ANY_KEY_PRESSED (keys)

extern uint8_t keys;
extern uint8_t previous_keys;

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



#endif // _COMMON_H