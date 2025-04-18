#include "saf_rng.h"
#include <stdint.h>

/*-------------- estado ---------------------------------------------------*/
static uint64_t s = 0x243F6A8885A308D3ULL;   /* estado T‑function            */
static uint64_t w = 0;                       /* contador Weyl               */
#define INC 0x9E3779B97F4A7C15ULL            /* φ·2^64                      */

/*-------------- núcleo ----------------------------------------------------*/
/* T‑function con multiplicación impar: difunde el bit 0 */
static inline uint64_t tfunc(uint64_t x){
    const uint64_t C = 0xDA3E39CB94B95BDBULL;
    return x + ((x<<13) ^ (x>>7)) + C;
}
static inline uint64_t mix(uint64_t z){
    z ^= z >> 30;  z *= 0xBF58476D1CE4E5B9ULL;
    z ^= z >> 27;  z *= 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}
uint64_t saf_rng_u64(void){
    s = tfunc(s);
    w += INC;
    uint64_t z = (s ^ w) ^ 0x1ULL;   /* ← añade ‘1’ para romper paridad exacta */
    return splitmix(z);              /* difunde con SplitMix64     
}
