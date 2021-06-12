
#include <stdint.h>

#include "common.h"
#include "torch_sprite_data.h"

// ====================================================================

// Sprite Circle Left Edge mask OAM arrangements


// Block of sprites that do NOT change (Large circle)
const uint8_t spr_circle_lg_fixed[] = {
    // Row 0
    //  x   y   id  flip
    C_L(0U, 0U, 0U, FLIP_NO),
    C_L(1U, 0U, 1U, FLIP_NO),
    C_L(2U, 0U, 2U, FLIP_NO),
    C_L(3U, 0U, 3U, FLIP_NO),
    C_L(4U, 0U, 4U, FLIP_NO),

    // Row 1
    C_L(0U, 1U, 0U, FLIP_NO),

    // Row 2
    C_L(0U, 2U, 0U, FLIP_NO),

    // Row 3
    //  x   y   id  flip
    C_L(0U, 3U, 0U, FLIP_V),
    C_L(1U, 3U, 1U, FLIP_V),
    C_L(2U, 3U, 2U, FLIP_V),
    C_L(3U, 3U, 3U, FLIP_V),
    C_L(4U, 3U, 4U, FLIP_V)
};


// Center block of sprites that do change (Small circle)
const uint8_t spr_circle_sm[] = {
    // Row 1
    //  x   y   id  flip
    C_S(1U, 1U, 0U, FLIP_NO),
    C_S(2U, 1U, 0U, FLIP_NO),
    C_S(3U, 1U, 1U, FLIP_NO),
    C_S(4U, 1U, 2U, FLIP_NO),
    // Row 2
    C_S(1U, 2U, 0U, FLIP_NO),
    C_S(2U, 2U, 0U, FLIP_NO),
    C_S(3U, 2U, 1U, FLIP_V),
    C_S(4U, 2U, 2U, FLIP_V)
};


// Center block of sprites that do change (Large circle)
const uint8_t spr_circle_lg[] = {
    // Row 1
    //  x   y   id  flip
    C_L(1U, 1U, 5U, FLIP_NO),
    C_HIDE(),
    C_HIDE(),
    C_HIDE(),
    // Row 2
    C_L(1U, 2U, 5U, FLIP_V),
    C_HIDE(),
    C_HIDE(),
    C_HIDE()
};


#define OAM_BYTES_PER_SPRITE    4U
#define SPR_COUNT_LG_FIXED      (sizeof(spr_circle_lg_fixed) / OAM_BYTES_PER_SPRITE)
#define SPR_COUNT_LG            (sizeof(spr_circle_lg) / OAM_BYTES_PER_SPRITE)
#define SPR_COUNT_SM            (sizeof(spr_circle_sm) / OAM_BYTES_PER_SPRITE)

#define SPR_COUNT_FIXED_START   (40u - SPR_COUNT_LG_FIXED - SPR_COUNT_LG)

const uint8_t spr_count_fixed_start  = SPR_COUNT_FIXED_START;  // Align with end of OAM
const uint8_t spr_count_change_start = (SPR_COUNT_FIXED_START + (SPR_COUNT_LG_FIXED));

const uint8_t spr_circle_lg_fixed_size = sizeof(spr_circle_lg_fixed);
const uint8_t spr_circle_lg_size = sizeof(spr_circle_sm);
const uint8_t spr_circle_sm_size = sizeof(spr_circle_lg);