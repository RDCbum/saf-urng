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
/* Permutación de salida PCG‑XSL‑RR (64 bits, sin sesgo de bit 0) */
static inline uint64_t pcg_output(uint64_t x){
    x ^= x >> 27;  x *= 0x3C79AC492BA7B653ULL;   /* multiply 1 (impar) */
    x ^= x >> 33;  x *= 0x1C69B3F74AC4AE35ULL;   /* multiply 2 (impar) */
    x ^= x >> 27;
    uint64_t rot = x >> 59;                      /* usa 5 bits altos   */
    return (x >> rot) | (x << ((64 - rot) & 63));
}


/*------------- API -------------------------------------------------------*/
void saf_rng_seed(uint64_t seed) { s = seed; w = seed ^ INC; }

uint64_t saf_rng_u64(void){
    s = tfunc(s);     /* paso T‑function */
    w += INC;         /* paso Weyl       */
    return pcg_output(s + w);   /* ← SUMA  + permutación PCG */
}

uint32_t saf_rng_u32(void) { return (uint32_t)saf_rng_u64(); }
float     saf_rng_f32(void) { return (saf_rng_u32() >> 8) * (1.0f/16777216.0f); }
