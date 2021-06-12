
#ifndef _TORCH_WIN_DATA_H
#define _TORCH_WIN_DATA_H

#define Y_SIZE  16u // +/- center, so 2x for full size
#define Y_GROW  16u
#define Y_START (uint8_t)((144u / 2u) - Y_SIZE)
#define Y_END   (uint8_t)((144u / 2u) + Y_SIZE) // + 1 // +1 for proper centering <- not anymore

#define X_SIZE  16u // +/- center, so 2x for full size
#define X_GROW  16u
#define X_START (uint8_t)((160u / 2u) - X_SIZE)
#define X_END   (uint8_t)((160u / 2u) + X_SIZE) // + 1 // +1 for proper centering <- not anymore


extern const uint8_t __at(0x200) X_END_LUT_SM[];
extern const uint8_t __at(0x280) X_END_LUT_LG[];


 #endif // _TORCH_WIN_DATA_H