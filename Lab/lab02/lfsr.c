#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lfsr.h"

void lfsr_calculate(uint16_t *reg) {
	uint16_t tap = ((*reg >> 15) & 1) ^ ((*reg >> 13) & 1) ^ ((*reg >> 12) & 1) ^ ((*reg >> 10) & 1);
	*reg = (*reg << 1) | tap ;
	//第二种
	// uint16_t tap = ((*reg >> 0) ^ (*reg >> 2) ^ (*reg >> 3) ^ (*reg >> 5)) & 1;
	// *reg = (*reg >> 1) | (tap << 15);
}
