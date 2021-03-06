
#include <stdint.h>

#include "common.h"
#include "torch_win_data.h"

// ====================================================================

// Window Circle Right Edge MASK LUT
// See xls file

// 128 bit aligned
// Relies on altered CODE location (moving it from 0x0200): -Wl-b_CODE=0x0300

const uint8_t __at(0x200) X_END_LUT_SM[] = {
 0x57u,
 0x5Cu, 0x5Eu, 0x60u, 0x61u, 0x62u, 0x63u, 0x64u, 0x64u, 0x65u, 0x65u,
 0x66u, 0x66u, 0x66u, 0x66u, 0x66u, 
 0x66u, // mid-point
 // 0x66u, // removed for roundness fix
 0x66u, 0x66u, 0x66u, 0x66u,
 0x65u, 0x65u, 0x64u, 0x64u, 0x63u, 0x62u, 0x61u, 0x60u, 0x5Eu, 0x5Cu,
 0x57u
 };

// 128 bit aligned
const uint8_t __at(0x280) X_END_LUT_LG[] = {
 0x57u - 8u,  // First line, a little fix for roundness
 0x5Eu, 0x62u, 0x64u, 0x66u, 0x68u, 0x69u, 0x6Au, 0x6Cu, 0x6Du, 0x6Eu,
 0x6Fu, 0x6Fu, 0x70u, 0x71u, 0x72u, 0x72u, 0x73u, 0x73u, 0x74u, 0x74u, 0x75u, 
 0x75u, 0x75u, 0x75u, 0x76u, 0x76u, 0x76u, 0x76u, 0x76u, 0x76u, 0x76u, 
 0x76u, // mid-point
 // 0x76u,  // removed for roundness fix
 0x76u, 0x76u, 0x76u, 0x76u, 0x76u, 0x76u, 0x75u, 0x75u, 0x75u, 0x75u,
 0x74u, 0x74u, 0x73u, 0x73u, 0x72u, 0x72u, 0x71u, 0x70u, 0x6Fu, 0x6Fu, 0x6Eu,
 0x6Du, 0x6Cu, 0x6Au, 0x69u, 0x68u, 0x66u, 0x64u, 0x62u, 0x5Eu,
 0x57u - 8u  // Last line, a little fix for roundness
 };