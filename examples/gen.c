#include <stdlib.h>
#include <stdio.h>
#include "../src/saf_rng.h"

int main(int argc, char **argv){
    uint64_t n = (argc>1)?strtoull(argv[1],0,10):134217728ULL; /* 1 GiB */
    saf_rng_seed(0xCAFEBABE);
    for(uint64_t i=0;i<n;i++){
        uint64_t z = saf_rng_u64();
        fwrite(&z,8,1,stdout);
    }
    return 0;
}
