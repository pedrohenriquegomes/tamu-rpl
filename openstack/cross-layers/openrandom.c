#include <stdlib.h>
#include "opendefs.h"
#include "openrandom.h"
#include "idmanager.h"

//=========================== variables =======================================

random_vars_t random_vars;

//=========================== prototypes ======================================

//=========================== public ==========================================

void openrandom_init() {
   // seed the random number generator with the last 2 bytes of the MAC address
   random_vars.shift_reg  = 0;
   random_vars.shift_reg += idmanager_getMyID(ADDR_16B)->addr_16b[0]*256;
   random_vars.shift_reg += idmanager_getMyID(ADDR_16B)->addr_16b[1];
}

uint16_t openrandom_get16b() {
   uint8_t  i;
   uint16_t random_value;
   random_value = 0;
   for(i=0;i<16;i++) {
      // Galois shift register
      // taps: 16 14 13 11
      // characteristic polynomial: x^16 + x^14 + x^13 + x^11 + 1
      random_value          |= (random_vars.shift_reg & 0x01)<<i;
      random_vars.shift_reg  = (random_vars.shift_reg>>1)^(-(int16_t)(random_vars.shift_reg & 1)&0xb400);
   }
   return random_value;
}

uint16_t openrandom_getBeta(uint8_t alpha, uint8_t beta) {

   // Beta(alpha,beta) can be obtained choosing the alpha-th smallest of alpha+beta-1 uniform
   // random variable samples. alpha + beta - 1 has to be less or equal to 256

   // max samples = 256
   // this is a waste of memory for now, we are using 1/4 KB!
   uint16_t samples[256];

   uint8_t num_samples = alpha + beta - 1;
   uint8_t i, j, k;
   uint16_t temp;

   // generate samples
   for (i = 0; i < num_samples; i++) {
      samples[i] = openrandom_get16b();
   }

   // sort the samples (lets start with shellsort, which is efficient for embedded
   // according to  http://embeddedgurus.com/stack-overflow/2009/03/sorting-in-embedded-systems/

   // gaps as (3^k - 1)/2, not greater than N/3
   // pseudocode from https://en.wikipedia.org/wiki/Shellsort
   uint8_t gaps[4] = {40, 13, 4, 1};
   
   // let's find where to start in the gaps array
   for (i = 0; gaps[i] > num_samples; i++);
   
   for (; i < 4; i++) {
      for (j = gaps[i]; j < num_samples; j++) {
         temp = samples[j];
         for (k = j; (k >= gaps[i]) && (samples[k - gaps[i]] > temp); k -= gaps[i]) {       
            samples[k] = samples[k - gaps[i]];
         }
         samples[k] = temp;
      }
   }
   
   return samples[alpha-1];
}

//=========================== private =========================================