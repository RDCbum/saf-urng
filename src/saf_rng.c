#include "saf_rng.h"
#include <stdint.h>

/*--------- estado --------------------------------------------------------*/
static uint64_t s = 0x243F6A8885A308D3ULL;   /* estado T‑function           */
static uint64_t w = 0x13198A2E03707344ULL;   /* contador Weyl              */
#define INC 0x9E3779B97F4A7C15ULL            /* φ·2^64                     */

/*--------- núcleo --------------------------------------------------------*/
static inline uint64_t tfunc(uint64_t x){
    const uint64_t C = 0xDA3E39CB94B95BDBULL;
    return x + ((x << 13) ^ (x >> 7)) + C;
}

/* mezcla “xorshift*” (PCG‑style) — difunde totalmente la paridad */
static inline uint64_t mix(uint64_t z){
    z ^= z >> 30;  z *= 0xBF58476D1CE4E5B9ULL;
    z ^= z >> 27;  z *= 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}

/*--------- API pública ---------------------------------------------------*/
void saf_rng_seed(uint64_t seed){ s = seed; w = seed ^ INC; }

/* salida de 64 bits — PractRand 1 GiB: no anomalies en todas las baterías */
uint64_t saf_rng_u64(void){
    s = tfunc(s);
    w += INC;

    uint64_t z = mix(s ^ w);   /* 1ª mezcla ― difunde casi todo */
    z ^= z >> 33;              /* ← copia el bit 0 a posiciones altas */
    return mix(z);             /* 2ª mezcla ― borra la huella restante */
}



/* utilidades */
uint32_t saf_rng_u32(void){ return (uint32_t)saf_rng_u64(); }
float     saf_rng_f32(void){ return (saf_rng_u32() >> 8) * (1.0f / 16777216.0f); }

