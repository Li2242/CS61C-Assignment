#include "transpose.h"

/* The naive transpose function as a reference. */
void transpose_naive(int n, int blocksize, int *dst, int *src) {
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            dst[y + x * n] = src[x + y * n];
        }
    }
}

/* Implement cache blocking below. You should NOT assume that n is a
 * multiple of the block size. */
void transpose_blocking(int n, int blocksize, int *dst, int *src) {
    for(int block_x = 0; block_x < n; block_x+=blocksize){
		for(int block_y = 0; block_y < n; block_y+=blocksize){

			int max_x = (block_x + blocksize < n) ? block_x + blocksize : n;
            int max_y = (block_y + blocksize < n) ? block_y + blocksize : n;
			
			for(int x = block_x; x < max_x ; x++){
				for(int y = block_y; y < max_y; y++){
					dst[y + x * n] = src[x + y * n];
				}
			}
		}
	}
}
