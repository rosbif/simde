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
  const int bits_set =
    ((imm8 >> 7) & 1) +
    ((imm8 >> 6) & 1) +
    ((imm8 >> 5) & 1) +
    ((imm8 >> 4) & 1) +
    ((imm8 >> 3) & 1) +
    ((imm8 >> 2) & 1) +
    ((imm8 >> 1) & 1) +
    ((imm8 >> 0) & 1);
  const int negate = bits_set > 4;

  if (negate) imm8 = ~imm8;

  r = zero;

  if (imm8 & 0xc0) {
    t = simde_mm_and_si128(a, b);
    if ((imm8 & 0xc0) == 0xc0) r = simde_mm_or_si128(r, t);
    else if (imm8 & 0x80)      r = simde_mm_or_si128(r,    simde_mm_and_si128(c, t));
    else                       r = simde_mm_or_si128(r, simde_mm_andnot_si128(c, t));
  }

  if (imm8 & 0x30) {
    t = simde_mm_andnot_si128(b, a);
    if ((imm8 & 0x30) == 0x30) r = simde_mm_or_si128(r, t);
    else if (imm8 & 0x20)      r = simde_mm_or_si128(r,    simde_mm_and_si128(c, t));
    else                       r = simde_mm_or_si128(r, simde_mm_andnot_si128(c, t));
  }

  if (imm8 & 0x0c) {
    t = simde_mm_andnot_si128(a, b);
    if ((imm8 & 0x0c) == 0x0c) r = simde_mm_or_si128(r, t);
    else if (imm8 & 0x08)      r = simde_mm_or_si128(r,    simde_mm_and_si128(c, t));
    else                       r = simde_mm_or_si128(r, simde_mm_andnot_si128(c, t));
  }

  if (imm8 & 0x03) {
    t = simde_mm_xor_si128(simde_mm_or_si128(a, b), ff);
    if ((imm8 & 0x03) == 0x03) r = simde_mm_or_si128(r, t);
    else if (imm8 & 0x02)      r = simde_mm_or_si128(r,    simde_mm_and_si128(c, t));
    else                       r = simde_mm_or_si128(r, simde_mm_andnot_si128(c, t));
  }

  if (negate)                  r = simde_mm_xor_si128(r, ff);

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
