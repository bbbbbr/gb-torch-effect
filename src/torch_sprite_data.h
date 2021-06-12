
#ifndef _TORCH_SPRITE_DATA_H
#define _TORCH_SPRITE_DATA_H
// ====================================================================

// Sprite Circle Left edge

#define FLIP_NO 0x00U
#define FLIP_V  0x40U
#define SPR_MASK_ST_X     (6 * 8)   // Starts 5 tiles from Left edge
#define SPR_MASK_ST_Y     ((3.5 * 16) + 1)// Starts 3.5 tiles from Top edge + a small fudge factor
#define C_S(x,y,id,vflip) (y * 16u) + SPR_MASK_ST_Y, (x * 8u) + SPR_MASK_ST_X, (id * 2u) + SPR_TILE_START_MASK_SM, vflip
#define C_L(x,y,id,vflip) (y * 16u) + SPR_MASK_ST_Y, (x * 8u) + SPR_MASK_ST_X, (id * 2u) + SPR_TILE_START_MASK_LG, vflip
#define C_HIDE()           0,0,0,0  // Sprite in non-visible position


extern const uint8_t spr_circle_lg_fixed[];
extern const uint8_t spr_circle_sm[];
extern const uint8_t spr_circle_lg[];

extern const uint8_t spr_count_fixed_start;
extern const uint8_t spr_count_change_start;

extern const uint8_t spr_circle_lg_fixed_size;
extern const uint8_t spr_circle_lg_size;
extern const uint8_t spr_circle_sm_size;


 #endif // _TORCH_SPRITE_DATA_H