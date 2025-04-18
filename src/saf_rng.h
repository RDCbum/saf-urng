#ifndef SAF_RNG_H
#define SAF_RNG_H
#include <stdint.h>
void     saf_rng_seed(uint64_t seed);
uint64_t saf_rng_u64(void);
uint32_t saf_rng_u32(void);
float    saf_rng_f32(void);   /* 0 ≤ x < 1 */
#endif
