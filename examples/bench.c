#include <stdio.h>
#include <time.h>
#include "../src/saf_rng.h"

int main(void){
    const uint64_t N = 100000000;   /* 100 M palabras = 800 MB */
    saf_rng_seed(0xDEADBEEF);
    clock_t t0 = clock();
    for(uint64_t i=0;i<N;i++) saf_rng_u64();
    double sec = (clock()-t0)/(double)CLOCKS_PER_SEC;
    printf("Generated %.1f MB in %.3f s  ->  %.2f MB/s\n",
           N*8.0/1e6, sec, N*8.0/1e6 / sec);
    return 0;
}
