#include <stdio.h>
#include "bit_ops.h"

// Return the nth bit of x.
// Assume 0 <= n <= 31
unsigned get_bit(unsigned x,
                 unsigned n) {
    unsigned temp = (x >> n) & 1;
    return temp;
}
// Set the nth bit of the value of x to v.
// Assume 0 <= n <= 31, and v is 0 or 1
void set_bit(unsigned * x,
             unsigned n,
             unsigned v) {
    if(v == 1){
		*x = *x | (1 << n);
	}else{
		*x = *x & ~(1 << n); // ~(1 << n) 生成一个掩码：第n位为0，其他位为1
	}
}
// Flip the nth bit of the value of x.
// Assume 0 <= n <= 31
void flip_bit(unsigned * x,
              unsigned n) {

	*x = *x ^ (1 << n);
}

