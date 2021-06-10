/* SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Copyright:
 *   2020      Evan Nemerson <evan@nemerson.com>
 *   2021      Christopher Moore <moore@free.fr>
 */

#if !defined(SIMDE_X86_AVX512_TERNARYLOGIC_H)
#define SIMDE_X86_AVX512_TERNARYLOGIC_H

#include "types.h"

HEDLEY_DIAGNOSTIC_PUSH
SIMDE_DISABLE_UNWANTED_DIAGNOSTICS
SIMDE_BEGIN_DECLS_

SIMDE_FUNCTION_ATTRIBUTES
simde__m128i
simde_mm_ternarylogic_epi32(simde__m128i a, simde__m128i b, simde__m128i c, int imm8)
  SIMDE_REQUIRE_CONSTANT_RANGE(imm8, 0, 255) {
  simde__m128i r, t;
  const simde__m128i zero = simde_mm_setzero_si128();
  const simde__m128i ff = simde_mm_cmpeq_epi8(zero, zero);
  int to_do, mask;

  to_do = imm8;

  mask = 0xFF;
  if ((to_do & mask) == mask) {
    r = ff;
    to_do &= ~mask;
  }
  else r = zero;

  mask = 0xF0;
  if ((to_do & mask) && ((imm8 & mask) == mask)) {
    r = a;
    to_do &= ~mask;
  }

  mask = 0xCC;
  if ((to_do & mask) && ((imm8 & mask) == mask)) {
    r = simde_mm_or_si128(r, b);
    to_do &= ~mask;
  }

  mask = 0xAA;
  if ((to_do & mask) && ((imm8 & mask) == mask)) {
    r = simde_mm_or_si128(r, c);
    to_do &= ~mask;
  }

  mask = 0x0F;
  if ((to_do & mask) && ((imm8 & mask) == mask)) {
    r = simde_mm_or_si128(r, simde_mm_xor_si128(a, ff));
    to_do &= ~mask;
  }

  mask = 0x33;
  if ((to_do & mask) && ((imm8 & mask) == mask)) {
    r = simde_mm_or_si128(r, simde_mm_xor_si128(b, ff));
    to_do &= ~mask;
  }

  mask = 0x55;
  if ((to_do & mask) && ((imm8 & mask) == mask)) {
    r = simde_mm_or_si128(r, simde_mm_xor_si128(c, ff));
    to_do &= ~mask;
  }

  mask = 0x3C;
  if ((to_do & mask) && ((imm8 & mask) == mask)) {
    r = simde_mm_or_si128(r, simde_mm_xor_si128(a, b));
    to_do &= ~mask;
  }

  mask = 0x5A;
  if ((to_do & mask) && ((imm8 & mask) == mask)) {
    r = simde_mm_or_si128(r, simde_mm_xor_si128(a, c));
    to_do &= ~mask;
  }

  mask = 0x66;
  if ((to_do & mask) && ((imm8 & mask) == mask)) {
    r = simde_mm_or_si128(r, simde_mm_xor_si128(b, c));
    to_do &= ~mask;
  }

  mask = 0xA0;
  if ((to_do & mask) && ((imm8 & mask) == mask)) {
    r = simde_mm_or_si128(r, simde_mm_and_si128(a, c));
    to_do &= ~mask;
  }

  mask = 0x50;
  if ((to_do & mask) && ((imm8 & mask) == mask)) {
    r = simde_mm_or_si128(r, simde_mm_andnot_si128(c, a));
    to_do &= ~mask;
  }

  mask = 0x0A;
  if ((to_do & mask) && ((imm8 & mask) == mask)) {
    r = simde_mm_or_si128(r, simde_mm_andnot_si128(a, c));
    to_do &= ~mask;
  }

  mask = 0x88;
  if ((to_do & mask) && ((imm8 & mask) == mask)) {
    r = simde_mm_or_si128(r, simde_mm_and_si128(b, c));
    to_do &= ~mask;
  }

  mask = 0x44;
  if ((to_do & mask) && ((imm8 & mask) == mask)) {
    r = simde_mm_or_si128(r, simde_mm_andnot_si128(c, b));
    to_do &= ~mask;
  }

  mask = 0x22;
  if ((to_do & mask) && ((imm8 & mask) == mask)) {
    r = simde_mm_or_si128(r, simde_mm_andnot_si128(b, c));
    to_do &= ~mask;
  }

  if (to_do & 0xc0) {
    t = simde_mm_and_si128(a, b);
    if ((to_do & 0xc0) == 0xc0) r = simde_mm_or_si128(r, t);
    else if (to_do & 0x80)      r = simde_mm_or_si128(r,    simde_mm_and_si128(c, t));
    else                        r = simde_mm_or_si128(r, simde_mm_andnot_si128(c, t));
  }

  if (to_do & 0x30) {
    t = simde_mm_andnot_si128(b, a);
    if ((to_do & 0x30) == 0x30) r = simde_mm_or_si128(r, t);
    else if (to_do & 0x20)      r = simde_mm_or_si128(r,    simde_mm_and_si128(c, t));
    else                        r = simde_mm_or_si128(r, simde_mm_andnot_si128(c, t));
  }

  if (to_do & 0x0c) {
    t = simde_mm_andnot_si128(a, b);
    if ((to_do & 0x0c) == 0x0c) r = simde_mm_or_si128(r, t);
    else if (to_do & 0x08)      r = simde_mm_or_si128(r,    simde_mm_and_si128(c, t));
    else                        r = simde_mm_or_si128(r, simde_mm_andnot_si128(c, t));
  }

  if (to_do & 0x03) {
    t = simde_mm_xor_si128(simde_mm_or_si128(a, b), ff);
    if ((to_do & 0x03) == 0x03) r = simde_mm_or_si128(r, t);
    else if (to_do & 0x02)      r = simde_mm_or_si128(r,    simde_mm_and_si128(c, t));
    else                        r = simde_mm_or_si128(r, simde_mm_andnot_si128(c, t));
  }
  return r;
}
#if defined(SIMDE_X86_AVX512VL_NATIVE) && defined(SIMDE_X86_AVX512F_NATIVE)
  #define simde_mm_ternarylogic_epi32(a, b, c, imm8) _mm_ternarylogic_epi32(a, b, c, imm8)
#endif
#if defined(SIMDE_X86_AVX512VL_ENABLE_NATIVE_ALIASES) && defined(SIMDE_X86_AVX512F_ENABLE_NATIVE_ALIASES)
  #undef _mm_ternarylogic_epi32
  #define _mm_ternarylogic_epi32(a, b, c, imm8) simde_mm_ternarylogic_epi32(a, b, c, imm8)
#endif

SIMDE_END_DECLS_
HEDLEY_DIAGNOSTIC_POP

#endif /* !defined(SIMDE_X86_AVX512_TERNARYLOGIC_H) */
