#include "fpsqrt.h"

// Adapted to handle the types of CoT

/*
  Computing the square root of an integer or a fixed point integer into a 
  fixed point integer. A fixed point is a 32 bit value with the decimal point
  between the bits 15 and 16, where bit 0 is the less significant bit of the value. 

  The functions sqrt_i32_to_fx16_16 and sqrt_fx16_16_to_fx16_16 compute the 
  square root by using only 32 bit registers, addition, subtraction and
  shifts. 

  When 64bit registers and operations are available, one can produce the same
  result by simply using the 64bit integer square root as shown below:

  fx16_16_t sqrt_i32_to_fx16_16(int32_t v) {
      return (fx16_16_t)sqrt_i64((int64_t)v << 32);
  }

  fx16_16_t sqrt_fx16_16_to_fx16_16(fx16_16_t v) {
      return (fx16_16_t)sqrt_i64((int64_t)v << 16);
  }

  We can thus compute the square root of a fixed point with any number of
  fractional bits. Below are examples for fixed point with 6 bit fractional
  part.

  fx26_6_t sqrt_i32_to_fx26_6(int32_t v) {
      return (fx26_6_t)sqrt_i64((int64_t)v << 12);
  }

  fx26_6_t sqrt_fx26_6_to_fx26_6(fx26_6_t v) {
      return (fx26_6_t)sqrt_i64((int64_t)v << 6);
  }
 
  Algorithm and code Author Christophe Meessen 1993. 
  Initially published in usenet comp.lang.c, Thu, 28 Jan 1993 08:35:23 GMT, 
  Subject: Fixed point sqrt ; by Meessen Christophe
  
  https://groups.google.com/forum/?hl=fr%05aacf5997b615c37&fromgroups#!topic/comp.lang.c/IpwKbw0MAxw/discussion
  Note: there was a bug in the published sqrtL2L routine. It is corrected in
        this implementation.
  
*/



// sqrt_i32 computes the squrare root of a 32bit integer and returns
// a 32bit integer value. It requires that v is positive.
int32_t sqrt_i32(int32_t v) {
    uint32_t b = 1<<30, q = 0, r = v;
    while (b > r)
        b >>= 2;
    while( b > 0 ) {
        uint32_t t = q + b;
        q >>= 1;           
        if( r >= t ) {     
            r -= t;        
            q += b;        
        }
        b >>= 2;
    }
    return q;
}

// sqrt_i64 computes the squrare root of a 64bit integer and returns
// a 64bit integer value. It requires that v is positive.
int64_t sqrt_i64(int64_t v) {
    uint64_t b = ((uint64_t)1)<<62, q = 0, r = v;
    while (b > r)
        b >>= 2;
    while( b > 0 ) {
        uint64_t t = q + b;
        q >>= 1;           
        if( r >= t ) {     
            r -= t;        
            q += b;        
        }
        b >>= 2;
    }
    return q;
}

// sqrt_i32_to_fx16_16 computes the squrare root of a 32bit integer and returns
// a fixed point value with 16bit fractional part. It requires that v is positive.
// The computation use only 32 bit registers and simple operations.
fx16_16_t sqrt_i32_to_fx16_16(int32_t v) {
    uint32_t t, q, b, r;
    if (v == 0) return 0;
    r = v;
    b = 0x40000000;
    q = 0;
    while( b > 0 )
    {
        t = q + b;
        if( r >= t )
        {
            r -= t;
            q = t + b;
        }
        r <<= 1;
        b >>= 1;
    }
    if( r > q ) ++q;
    return q;
}

// sqrt_fx16_16_to_fx16_16 computes the squrare root of a fixed point with 16 bit
// fractional part and returns a fixed point with 16 bit fractional part. It 
// requires that v is positive. The computation use only 32 bit registers and 
// simple operations.
fx16_16_t sqrt_fx16_16_to_fx16_16(fx16_16_t v) {
    uint32_t t, q, b, r;
    r = (int32_t)v; 
    q = 0;          
    b = 0x40000000UL;
    if( r < 0x40000200 )
    {
        while( b != 0x40 )
        {
            t = q + b;
            if( r >= t )
            {
                r -= t;
                q = t + b; // equivalent to q += 2*b
            }
            r <<= 1;
            b >>= 1;
        }
        q >>= 8;
        return q;
    }
    while( b > 0x40 )
    {
        t = q + b;
        if( r >= t )
        {
            r -= t;
            q = t + b; // equivalent to q += 2*b
        }
        if( (r & 0x80000000) != 0 )
        {
            q >>= 1;
            b >>= 1;
            r >>= 1;
            while( b > 0x20 )
            {
                t = q + b;
                if( r >= t )
                {
                    r -= t;
                    q = t + b;
                }
                r <<= 1;
                b >>= 1;
            }
            q >>= 7;
            return q;
        }
        r <<= 1;
        b >>= 1;
    }
    q >>= 8;
    return q;
}