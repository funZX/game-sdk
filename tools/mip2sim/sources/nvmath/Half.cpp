// Branch-free implementation of half-precision (16 bit) floating point
// Copyright 2006 Mike Acton <macton@gmail.com>
// 
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included 
// in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE
//
// Half-precision floating point format
// ------------------------------------
//
//   | Field    | Last | First | Note
//   |----------|------|-------|----------
//   | Sign     | 15   | 15    |
//   | Exponent | 14   | 10    | Bias = 15
//   | Mantissa | 9    | 0     |
//
// Compiling
// ---------
//
//  Preferred compile flags for GCC: 
//     -O3 -fstrict-aliasing -std=c99 -pedantic -Wall -Wstrict-aliasing
//
//     This file is a C99 source file, intended to be compiled with a C99 
//     compliant compiler. However, for the moment it remains combatible
//     with C++98. Therefore if you are using a compiler that poorly implements
//     C standards (e.g. MSVC), it may be compiled as C++. This is not
//     guaranteed for future versions. 
//
// Features
// --------
//
//  * QNaN + <x>  = QNaN
//  * <x>  + +INF = +INF
//  * <x>  - -INF = -INF
//  * INF  - INF  = SNaN
//  * Denormalized values
//  * Difference of ZEROs is always +ZERO
//  * Sum round with guard + round + sticky bit (grs)
//  * And of course... no branching
// 
// Precision of Sum
// ----------------
//
//  (SUM)        uint16 z = half_add( x, y );
//  (DIFFERENCE) uint16 z = half_add( x, -y );
//
//     Will have exactly (0 ulps difference) the same result as:
//     (For 32 bit IEEE 784 floating point and same rounding mode)
//
//     union FLOAT_32
//     {
//       float    f32;
//       uint32 u32;
//     };
//
//     union FLOAT_32 fx = { .u32 = half_to_float( x ) };
//     union FLOAT_32 fy = { .u32 = half_to_float( y ) };
//     union FLOAT_32 fz = { .f32 = fx.f32 + fy.f32    };
//     uint16       z  = float_to_half( fz );
//

#include <nvmath/Half.h>
#include <stdio.h>

// Load immediate
static inline uint32 _uint32_li( uint32 a )
{
    return (a);
}

// Decrement
static inline uint32 _uint32_dec( uint32 a )
{
    return (a - 1);
}

// Increment
static inline uint32 _uint32_inc( uint32 a )
{
  return (a + 1);
}

// Complement
static inline uint32 _uint32_not( uint32 a )
{
    return (~a);
}

// Negate
static inline uint32 _uint32_neg( uint32 a )
{
#pragma warning(disable : 4146)     // unary minus operator applied to unsigned type, result still unsigned
    return (-a);
#pragma warning(default : 4146)
}

// Extend sign
static inline uint32 _uint32_ext( uint32 a )
{
    return (((int32)a)>>31);
}

// And
static inline uint32 _uint32_and( uint32 a, uint32 b )
{
    return (a & b);
}

// And with Complement
static inline uint32 _uint32_andc( uint32 a, uint32 b )
{
    return (a & ~b);
}

// Or
static inline uint32 _uint32_or( uint32 a, uint32 b )
{
    return (a | b);
}

// Shift Right Logical
static inline uint32 _uint32_srl( uint32 a, int sa )
{
    return (a >> sa);
}

// Shift Left Logical
static inline uint32 _uint32_sll( uint32 a, int sa )
{
    return (a << sa);
}

// Add
static inline uint32 _uint32_add( uint32 a, uint32 b )
{
    return (a + b);
}

// Subtract
static inline uint32 _uint32_sub( uint32 a, uint32 b )
{
    return (a - b);
}

// Select on Sign bit
static inline uint32 _uint32_sels( uint32 test, uint32 a, uint32 b )
{
    const uint32 mask   = _uint32_ext( test );
    const uint32 sel_a  = _uint32_and(  a,     mask  );
    const uint32 sel_b  = _uint32_andc( b,     mask  );
    const uint32 result = _uint32_or(   sel_a, sel_b );

    return (result);
}

// Load Immediate
static inline uint16 _uint16_li( uint16 a )
{
    return (a);
}

// Extend sign
static inline uint16 _uint16_ext( uint16 a )
{
    return (((int16)a)>>15);
}

// Negate
static inline uint16 _uint16_neg( uint16 a )
{
    return (-a);
}

// Complement
static inline uint16 _uint16_not( uint16 a )
{
    return (~a);
}

// Decrement
static inline uint16 _uint16_dec( uint16 a )
{
    return (a - 1);
}

// Shift Left Logical
static inline uint16 _uint16_sll( uint16 a, int sa )
{
    return (a << sa);
}

// Shift Right Logical
static inline uint16 _uint16_srl( uint16 a, int sa )
{
    return (a >> sa);
}

// Add
static inline uint16 _uint16_add( uint16 a, uint16 b )
{
    return (a + b);
}

// Subtract
static inline uint16 _uint16_sub( uint16 a, uint16 b )
{
    return (a - b);
}

// And
static inline uint16 _uint16_and( uint16 a, uint16 b )
{
    return (a & b);
}

// Or
static inline uint16 _uint16_or( uint16 a, uint16 b )
{
    return (a | b);
}

// Exclusive Or
static inline uint16 _uint16_xor( uint16 a, uint16 b )
{
    return (a ^ b);
}

// And with Complement
static inline uint16 _uint16_andc( uint16 a, uint16 b )
{
    return (a & ~b);
}

// And then Shift Right Logical
static inline uint16 _uint16_andsrl( uint16 a, uint16 b, int sa )
{
    return ((a & b) >> sa);
}

// Shift Right Logical then Mask
static inline uint16 _uint16_srlm( uint16 a, int sa, uint16 mask )
{
    return ((a >> sa) & mask);
}

// Add then Mask
static inline uint16 _uint16_addm( uint16 a, uint16 b, uint16 mask )
{
    return ((a + b) & mask);
}


// Select on Sign bit
static inline uint16 _uint16_sels( uint16 test, uint16 a, uint16 b )
{
    const uint16 mask   = _uint16_ext( test );
    const uint16 sel_a  = _uint16_and(  a,     mask  );
    const uint16 sel_b  = _uint16_andc( b,     mask  );
    const uint16 result = _uint16_or(   sel_a, sel_b );

    return (result);
}

#if NV_OS_XBOX
#include <PPCIntrinsics.h>
#elif NV_CC_MSVC

#include <intrin.h>
#pragma intrinsic(_BitScanReverse)

uint32 _uint32_nlz( uint32 x ) {
    unsigned long index;
    _BitScanReverse(&index, x);
    return 31 - index;
}
#endif


// Count Leading Zeros
static inline uint32 _uint32_cntlz( uint32 x )
{
#if NV_CC_GCC
    /* On PowerPC, this will map to insn: cntlzw */
    /* On Pentium, this will map to insn: clz    */
    uint32 is_x_nez_msb = _uint32_neg( x );
    uint32 nlz          = __builtin_clz( x );
    uint32 result       = _uint32_sels( is_x_nez_msb, nlz, 0x00000020 );
    return (result);
#elif NV_OS_XBOX
    // Xbox PPC has this as an intrinsic.
    return _CountLeadingZeros(x);
#elif NV_CC_MSVC
    uint32 is_x_nez_msb = _uint32_neg( x );
    uint32 nlz          = _uint32_nlz( x );
    uint32 result       = _uint32_sels( is_x_nez_msb, nlz, 0x00000020 );
    return (result);
#else
    const uint32 x0  = _uint32_srl(  x,  1 );
    const uint32 x1  = _uint32_or(   x,  x0 );
    const uint32 x2  = _uint32_srl(  x1, 2 );
    const uint32 x3  = _uint32_or(   x1, x2 );
    const uint32 x4  = _uint32_srl(  x3, 4 );
    const uint32 x5  = _uint32_or(   x3, x4 );
    const uint32 x6  = _uint32_srl(  x5, 8 );
    const uint32 x7  = _uint32_or(   x5, x6 );
    const uint32 x8  = _uint32_srl(  x7, 16 );
    const uint32 x9  = _uint32_or(   x7, x8 );
    const uint32 xA  = _uint32_not(  x9 );
    const uint32 xB  = _uint32_srl(  xA, 1 );
    const uint32 xC  = _uint32_and(  xB, 0x55555555 );
    const uint32 xD  = _uint32_sub(  xA, xC );
    const uint32 xE  = _uint32_and(  xD, 0x33333333 );
    const uint32 xF  = _uint32_srl(  xD, 2 );
    const uint32 x10 = _uint32_and(  xF, 0x33333333 );
    const uint32 x11 = _uint32_add(  xE, x10 );
    const uint32 x12 = _uint32_srl(  x11, 4 );
    const uint32 x13 = _uint32_add(  x11, x12 );
    const uint32 x14 = _uint32_and(  x13, 0x0f0f0f0f );
    const uint32 x15 = _uint32_srl(  x14, 8 );
    const uint32 x16 = _uint32_add(  x14, x15 );
    const uint32 x17 = _uint32_srl(  x16, 16 );
    const uint32 x18 = _uint32_add(  x16, x17 );
    const uint32 x19 = _uint32_and(  x18, 0x0000003f );
    return ( x19 );
#endif
}

// Count Leading Zeros
static inline uint16 _uint16_cntlz( uint16 x )
{
#ifdef __GNUC__
    /* On PowerPC, this will map to insn: cntlzw */
    /* On Pentium, this will map to insn: clz    */
    uint16 nlz32 = (uint16)_uint32_cntlz( (uint32)x );
    uint32 nlz   = _uint32_sub( nlz32, 16 );
    return (nlz);
#elif _NV_OS_XBOX_
    uint16 nlz32 = (uint16)_CountLeadingZeros( (uint32)x );
    return _uint32_sub( nlz32, 16);
#else
    const uint16 x0  = _uint16_srl(  x,  1 );
    const uint16 x1  = _uint16_or(   x,  x0 );
    const uint16 x2  = _uint16_srl(  x1, 2 );
    const uint16 x3  = _uint16_or(   x1, x2 );
    const uint16 x4  = _uint16_srl(  x3, 4 );
    const uint16 x5  = _uint16_or(   x3, x4 );
    const uint16 x6  = _uint16_srl(  x5, 8 );
    const uint16 x7  = _uint16_or(   x5, x6 );
    const uint16 x8  = _uint16_not(  x7 );
    const uint16 x9  = _uint16_srlm( x8, 1, 0x5555 );
    const uint16 xA  = _uint16_sub(  x8, x9 );
    const uint16 xB  = _uint16_and(  xA, 0x3333 );
    const uint16 xC  = _uint16_srlm( xA, 2, 0x3333 );
    const uint16 xD  = _uint16_add(  xB, xC );
    const uint16 xE  = _uint16_srl(  xD, 4 );
    const uint16 xF  = _uint16_addm( xD, xE, 0x0f0f );
    const uint16 x10 = _uint16_srl(  xF, 8 );
    const uint16 x11 = _uint16_addm( xF, x10, 0x001f );
    return ( x11 );
#endif
}

uint16
nv::half_from_float( uint32 f )
{
    const uint32 one                        = _uint32_li( 0x00000001 );
    const uint32 f_s_mask                   = _uint32_li( 0x80000000 );
    const uint32 f_e_mask                   = _uint32_li( 0x7f800000 );
    const uint32 f_m_mask                   = _uint32_li( 0x007fffff );
    const uint32 f_m_hidden_bit             = _uint32_li( 0x00800000 );
    const uint32 f_m_round_bit              = _uint32_li( 0x00001000 );
    const uint32 f_snan_mask                = _uint32_li( 0x7fc00000 );
    const uint32 f_e_pos                    = _uint32_li( 0x00000017 );
    const uint32 h_e_pos                    = _uint32_li( 0x0000000a );
    const uint32 h_e_mask                   = _uint32_li( 0x00007c00 );
    const uint32 h_snan_mask                = _uint32_li( 0x00007e00 );
    const uint32 h_e_mask_value             = _uint32_li( 0x0000001f );
    const uint32 f_h_s_pos_offset           = _uint32_li( 0x00000010 );
    const uint32 f_h_bias_offset            = _uint32_li( 0x00000070 );
    const uint32 f_h_m_pos_offset           = _uint32_li( 0x0000000d );
    const uint32 h_nan_min                  = _uint32_li( 0x00007c01 );
    const uint32 f_h_e_biased_flag          = _uint32_li( 0x0000008f );
    const uint32 f_s                        = _uint32_and( f,               f_s_mask         );
    const uint32 f_e                        = _uint32_and( f,               f_e_mask         );
    const uint16 h_s                        = _uint32_srl( f_s,             f_h_s_pos_offset );
    const uint32 f_m                        = _uint32_and( f,               f_m_mask         );
    const uint16 f_e_amount                 = _uint32_srl( f_e,             f_e_pos          );
    const uint32 f_e_half_bias              = _uint32_sub( f_e_amount,      f_h_bias_offset  );
    const uint32 f_snan                     = _uint32_and( f,               f_snan_mask      );
    const uint32 f_m_round_mask             = _uint32_and( f_m,             f_m_round_bit    );
    const uint32 f_m_round_offset           = _uint32_sll( f_m_round_mask,  one              );
    const uint32 f_m_rounded                = _uint32_add( f_m,             f_m_round_offset );
    const uint32 f_m_denorm_sa              = _uint32_sub( one,             f_e_half_bias    );
    const uint32 f_m_with_hidden            = _uint32_or(  f_m_rounded,     f_m_hidden_bit   );
    const uint32 f_m_denorm                 = _uint32_srl( f_m_with_hidden, f_m_denorm_sa    );
    const uint32 h_m_denorm                 = _uint32_srl( f_m_denorm,      f_h_m_pos_offset );
    const uint32 f_m_rounded_overflow       = _uint32_and( f_m_rounded,     f_m_hidden_bit   );
    const uint32 m_nan                      = _uint32_srl( f_m,             f_h_m_pos_offset );
    const uint32 h_em_nan                   = _uint32_or(  h_e_mask,        m_nan            );
    const uint32 h_e_norm_overflow_offset   = _uint32_inc( f_e_half_bias );
    const uint32 h_e_norm_overflow          = _uint32_sll( h_e_norm_overflow_offset, h_e_pos          );
    const uint32 h_e_norm                   = _uint32_sll( f_e_half_bias,            h_e_pos          );
    const uint32 h_m_norm                   = _uint32_srl( f_m_rounded,              f_h_m_pos_offset );
    const uint32 h_em_norm                  = _uint32_or(  h_e_norm,                 h_m_norm         );
    const uint32 is_h_ndenorm_msb           = _uint32_sub( f_h_bias_offset,   f_e_amount    );
    const uint32 is_f_e_flagged_msb         = _uint32_sub( f_h_e_biased_flag, f_e_half_bias );
    const uint32 is_h_denorm_msb            = _uint32_not( is_h_ndenorm_msb );
    const uint32 is_f_m_eqz_msb             = _uint32_dec( f_m   );
    const uint32 is_h_nan_eqz_msb           = _uint32_dec( m_nan );
    const uint32 is_f_inf_msb               = _uint32_and( is_f_e_flagged_msb, is_f_m_eqz_msb   );
    const uint32 is_f_nan_underflow_msb     = _uint32_and( is_f_e_flagged_msb, is_h_nan_eqz_msb );
    const uint32 is_e_overflow_msb          = _uint32_sub( h_e_mask_value,     f_e_half_bias    );
    const uint32 is_h_inf_msb               = _uint32_or(  is_e_overflow_msb,  is_f_inf_msb     );
    const uint32 is_f_nsnan_msb             = _uint32_sub( f_snan,             f_snan_mask      );
    const uint32 is_m_norm_overflow_msb     = _uint32_neg( f_m_rounded_overflow );
    const uint32 is_f_snan_msb              = _uint32_not( is_f_nsnan_msb );
    const uint32 h_em_overflow_result       = _uint32_sels( is_m_norm_overflow_msb, h_e_norm_overflow, h_em_norm                 );
    const uint32 h_em_nan_result            = _uint32_sels( is_f_e_flagged_msb,     h_em_nan,          h_em_overflow_result      );
    const uint32 h_em_nan_underflow_result  = _uint32_sels( is_f_nan_underflow_msb, h_nan_min,         h_em_nan_result           );
    const uint32 h_em_inf_result            = _uint32_sels( is_h_inf_msb,           h_e_mask,          h_em_nan_underflow_result );
    const uint32 h_em_denorm_result         = _uint32_sels( is_h_denorm_msb,        h_m_denorm,        h_em_inf_result           );
    const uint32 h_em_snan_result           = _uint32_sels( is_f_snan_msb,          h_snan_mask,       h_em_denorm_result        );
    const uint32 h_result                   = _uint32_or( h_s, h_em_snan_result );

    return (uint16)(h_result);
}

uint32 
nv::half_to_float( uint16 h )
{
    const uint32 h_e_mask              = _uint32_li( 0x00007c00 );
    const uint32 h_m_mask              = _uint32_li( 0x000003ff );
    const uint32 h_s_mask              = _uint32_li( 0x00008000 );
    const uint32 h_f_s_pos_offset      = _uint32_li( 0x00000010 );
    const uint32 h_f_e_pos_offset      = _uint32_li( 0x0000000d );
    const uint32 h_f_bias_offset       = _uint32_li( 0x0001c000 );
    const uint32 f_e_mask              = _uint32_li( 0x7f800000 );
    const uint32 f_m_mask              = _uint32_li( 0x007fffff );
    const uint32 h_f_e_denorm_bias     = _uint32_li( 0x0000007e );
    const uint32 h_f_m_denorm_sa_bias  = _uint32_li( 0x00000008 );
    const uint32 f_e_pos               = _uint32_li( 0x00000017 );
    const uint32 h_e_mask_minus_one    = _uint32_li( 0x00007bff );
    const uint32 h_e                   = _uint32_and( h, h_e_mask );
    const uint32 h_m                   = _uint32_and( h, h_m_mask );
    const uint32 h_s                   = _uint32_and( h, h_s_mask );
    const uint32 h_e_f_bias            = _uint32_add( h_e, h_f_bias_offset );
    const uint32 h_m_nlz               = _uint32_cntlz( h_m );
    const uint32 f_s                   = _uint32_sll( h_s,        h_f_s_pos_offset );
    const uint32 f_e                   = _uint32_sll( h_e_f_bias, h_f_e_pos_offset );
    const uint32 f_m                   = _uint32_sll( h_m,        h_f_e_pos_offset );
    const uint32 f_em                  = _uint32_or(  f_e,        f_m              );
    const uint32 h_f_m_sa              = _uint32_sub( h_m_nlz,             h_f_m_denorm_sa_bias );
    const uint32 f_e_denorm_unpacked   = _uint32_sub( h_f_e_denorm_bias,   h_f_m_sa             );
    const uint32 h_f_m                 = _uint32_sll( h_m,                 h_f_m_sa             );
    const uint32 f_m_denorm            = _uint32_and( h_f_m,               f_m_mask             );
    const uint32 f_e_denorm            = _uint32_sll( f_e_denorm_unpacked, f_e_pos              );
    const uint32 f_em_denorm           = _uint32_or(  f_e_denorm,          f_m_denorm           );
    const uint32 f_em_nan              = _uint32_or(  f_e_mask,            f_m                  );
    const uint32 is_e_eqz_msb          = _uint32_dec(  h_e );
    const uint32 is_m_nez_msb          = _uint32_neg(  h_m );
    const uint32 is_e_flagged_msb      = _uint32_sub(  h_e_mask_minus_one, h_e );
    const uint32 is_zero_msb           = _uint32_andc( is_e_eqz_msb,       is_m_nez_msb );
    const uint32 is_inf_msb            = _uint32_andc( is_e_flagged_msb,   is_m_nez_msb );
    const uint32 is_denorm_msb         = _uint32_and(  is_m_nez_msb,       is_e_eqz_msb );
    const uint32 is_nan_msb            = _uint32_and(  is_e_flagged_msb,   is_m_nez_msb ); 
    const uint32 is_zero               = _uint32_ext(  is_zero_msb );
    const uint32 f_zero_result         = _uint32_andc( f_em, is_zero );
    const uint32 f_denorm_result       = _uint32_sels( is_denorm_msb, f_em_denorm, f_zero_result );
    const uint32 f_inf_result          = _uint32_sels( is_inf_msb,    f_e_mask,    f_denorm_result );
    const uint32 f_nan_result          = _uint32_sels( is_nan_msb,    f_em_nan,    f_inf_result    );
    const uint32 f_result              = _uint32_or( f_s, f_nan_result );

    return (f_result);
}

uint32 
nv::fast_half_to_float( uint16 h )
{
    const uint32 h_e_mask              = _uint32_li( 0x00007c00 );
    const uint32 h_m_mask              = _uint32_li( 0x000003ff );
    const uint32 h_s_mask              = _uint32_li( 0x00008000 );
    const uint32 h_f_s_pos_offset      = _uint32_li( 0x00000010 );
    const uint32 h_f_e_pos_offset      = _uint32_li( 0x0000000d );
    const uint32 h_f_bias_offset       = _uint32_li( 0x0001c000 );
    const uint32 f_e_mask              = _uint32_li( 0x7f800000 );
    const uint32 f_m_mask              = _uint32_li( 0x007fffff );
    const uint32 h_f_e_denorm_bias     = _uint32_li( 0x0000007e );
    const uint32 h_f_m_denorm_sa_bias  = _uint32_li( 0x00000008 );
    const uint32 f_e_pos               = _uint32_li( 0x00000017 );
    const uint32 h_e_mask_minus_one    = _uint32_li( 0x00007bff );
    const uint32 h_e                   = _uint32_and( h, h_e_mask );
    const uint32 h_m                   = _uint32_and( h, h_m_mask );
    const uint32 h_s                   = _uint32_and( h, h_s_mask );
    const uint32 h_e_f_bias            = _uint32_add( h_e, h_f_bias_offset );
    const uint32 h_m_nlz               = _uint32_cntlz( h_m );
    const uint32 f_s                   = _uint32_sll( h_s,        h_f_s_pos_offset );
    const uint32 f_e                   = _uint32_sll( h_e_f_bias, h_f_e_pos_offset );
    const uint32 f_m                   = _uint32_sll( h_m,        h_f_e_pos_offset );
    const uint32 f_em                  = _uint32_or(  f_e,        f_m              );
    const uint32 h_f_m_sa              = _uint32_sub( h_m_nlz,             h_f_m_denorm_sa_bias );
    const uint32 f_e_denorm_unpacked   = _uint32_sub( h_f_e_denorm_bias,   h_f_m_sa             );
    const uint32 h_f_m                 = _uint32_sll( h_m,                 h_f_m_sa             );
    const uint32 f_m_denorm            = _uint32_and( h_f_m,               f_m_mask             );
    const uint32 f_e_denorm            = _uint32_sll( f_e_denorm_unpacked, f_e_pos              );
    const uint32 f_em_denorm           = _uint32_or(  f_e_denorm,          f_m_denorm           );
    const uint32 f_em_nan              = _uint32_or(  f_e_mask,            f_m                  );
    const uint32 is_e_eqz_msb          = _uint32_dec(  h_e );
    const uint32 is_m_nez_msb          = _uint32_neg(  h_m );
    const uint32 is_e_flagged_msb      = _uint32_sub(  h_e_mask_minus_one, h_e );
    const uint32 is_zero_msb           = _uint32_andc( is_e_eqz_msb,       is_m_nez_msb );
    const uint32 is_denorm_msb         = _uint32_and(  is_m_nez_msb,       is_e_eqz_msb );
    const uint32 is_zero               = _uint32_ext(  is_zero_msb );
    const uint32 f_zero_result         = _uint32_andc( f_em, is_zero );
    const uint32 f_denorm_result       = _uint32_sels( is_denorm_msb, f_em_denorm, f_zero_result );
    const uint32 f_result              = _uint32_or( f_s, f_denorm_result );

    return (f_result);
}
