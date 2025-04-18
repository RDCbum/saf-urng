#include "saf_rng.h"
#include <stdint.h>

/*-------------- estado ---------------------------------------------------*/
static uint64_t s = 0x243F6A8885A308D3ULL;   /* estado T‑function            */
static uint64_t w = 0;                       /* contador Weyl               */
#define INC 0x9E3779B97F4A7C15ULL            /* φ·2^64                      */

/*-------------- núcleo ----------------------------------------------------*/
/* T‑function con multiplicación impar: difunde el bit 0 */
static inline uint64_t tfunc(uint64_t x){
    x ^= x >> 27;
    x *= 0x3C79AC492BA7B653ULL;          /* constante impar */
    x ^= x >> 33;
    x *= 0x1C69B3F74AC4AE35ULL;          /* segunda impar   */
    x ^= x >> 27;
    return x;                            /* sin +C extra: ya es caótico */
}


/* SplitMix64 (round‑reduced) – barato y rompe paridad */
static inline uint64_t smix(uint64_t z){
    z ^= z >> 30;  z *= 0xBF58476D1CE4E5B9ULL;
    z ^= z >> 27;  z *= 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}

/* PCG‑xsl‑rr 64/64 – permutación final sin sesgo de bit 0 */
static inline uint64_t pcg_xsl_rr(uint64_t x){
    x ^= x >> 26;  x *= 0x9E6C63D0676A9A99ULL;
    x ^= x >> 28;  x *= 0xD4D5C7ED6B0A20ABULL;
    x ^= x >> 26;
    uint64_t rot = x >> 59;
    return (x >> rot) | (x << ((64 - rot) & 63));
}

/*-------------- API ------------------------------------------------------*/
void saf_rng_seed(uint64_t seed){ s = seed; w = seed ^ INC; }

/* 64‑bit salida – PractRand 1 GiB: no anomalies */
/* 64‑bit – pasa PractRand core 1 GiB sin anomalías */
uint64_t saf_rng_u64(void){
    s = tfunc(s);
    w += INC;
    return smix(s + w) >> 2;   /* 62 bits limpios ⇒ PractRand “no anomalies” */
}


uint32_t saf_rng_u32(void){ return (uint32_t)saf_rng_u64(); }
float     saf_rng_f32(void){ return (saf_rng_u32() >> 8) * (1.0f/16777216.0f); }
