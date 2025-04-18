#include "saf_rng.h"
#include <stdint.h>

/*------------- estado ----------------------------------------------------*/
static uint64_t s = 0x243F6A8885A308D3ULL;   /* T‑function state      */
static uint64_t w = 0;                       /* Weyl counter          */
#define INC 0x9E3779B97F4A7C15ULL            /* φ·2^64                */

/*------------- núcleo ----------------------------------------------------*/
static inline uint64_t tfunc(uint64_t x) {
    const uint64_t C = 0xDA3E39CB94B95BDBULL;
    return x + ((x << 13) ^ (x >> 7)) + C;
}
/* 64‑bit PCG “xorshift*” — rompe paridad en 1 sola llamada */
static inline uint64_t mix(uint64_t z) {
    z ^= z >> 30;  z *= 0xBF58476D1CE4E5B9ULL;
    z ^= z >> 27;  z *= 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}

/*------------- API -------------------------------------------------------*/
void saf_rng_seed(uint64_t seed) { s = seed; w = seed ^ INC; }

uint64_t saf_rng_u64(void) {
    s = tfunc(s);          /* avance T‑function                 */
    w += INC;              /* avance Weyl                       */
    return mix(s + w);     /* *** S U M A  + 1×mix  — probado *** */
}

uint32_t saf_rng_u32(void) { return (uint32_t)saf_rng_u64(); }
float     saf_rng_f32(void) { return (saf_rng_u32() >> 8) * (1.0f/16777216.0f); }
