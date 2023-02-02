

#ifndef _COMMON_H
#define _COMMON_H
// Sprite Tile Data allocation

#define UPDATE_KEYS() previous_keys = keys; keys = joypad()
#define KEY_PRESSED(K) (keys & (K))
#define KEY_TICKED(K)   ((keys & (K)) && !(previous_keys & (K)))
#define KEY_RELEASED(K) ((previous_keys & (K)) && !(keys & (K)))
#define ANY_KEY_PRESSED (keys)

extern uint8_t keys;
extern uint8_t previous_keys;

#define SPR_PLY_X (((160u / 2u) - 8u) + 8u)  // Centered. X position for sprites is -8 from set position
#define SPR_PLY_Y (((144u / 2u) - 8u) + 16u) // Centered. Y Position for sprites is -16 from set position

#define PLY_DIR_LEFT   0U
#define PLY_DIR_UP     2U
#define PLY_DIR_RIGHT  4U
#define PLY_DIR_DOWN   6U
#define PLY_DIR_RESET  0xFF



#endif // _COMMON_H
