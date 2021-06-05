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

#include <string.h>

#include "types.h"

HEDLEY_DIAGNOSTIC_PUSH
SIMDE_DISABLE_UNWANTED_DIAGNOSTICS
SIMDE_BEGIN_DECLS_

SIMDE_FUNCTION_ATTRIBUTES
simde__m128i
simde_mm_ternarylogic_epi32(simde__m128i a, simde__m128i b, simde__m128i c, int imm8)
  SIMDE_REQUIRE_CONSTANT_RANGE(imm8, 0, 255) {
  simde__m128i_private
    r_,
    a_ = simde__m128i_to_private(a),
    b_ = simde__m128i_to_private(b),
    c_ = simde__m128i_to_private(c);

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

  memset(&r_, 0, sizeof(r_));

  SIMDE_VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r_.u32) / sizeof(r_.u32[0])) ; i++) {
    if (imm8 & 0x80) r_.u32[i] |=  a_.u32[i] &  b_.u32[i] &  c_.u32[i];
    if (imm8 & 0x40) r_.u32[i] |=  a_.u32[i] &  b_.u32[i] & ~c_.u32[i];
    if (imm8 & 0x20) r_.u32[i] |=  a_.u32[i] & ~b_.u32[i] &  c_.u32[i];
    if (imm8 & 0x10) r_.u32[i] |=  a_.u32[i] & ~b_.u32[i] & ~c_.u32[i];
    if (imm8 & 0x08) r_.u32[i] |= ~a_.u32[i] &  b_.u32[i] &  c_.u32[i];
    if (imm8 & 0x04) r_.u32[i] |= ~a_.u32[i] &  b_.u32[i] & ~c_.u32[i];
    if (imm8 & 0x02) r_.u32[i] |= ~a_.u32[i] & ~b_.u32[i] &  c_.u32[i];
    if (imm8 & 0x01) r_.u32[i] |= ~a_.u32[i] & ~b_.u32[i] & ~c_.u32[i];
    if (negate)      r_.u32[i]  = ~r_.u32[i];
  }

  return simde__m128i_from_private(r_);
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
