/*
 * LibSylph Class Library
 * Copyright (C) 2012 Frank "SeySayux" Erens <seysayux@gmail.com>
 * Copyright (C) 2012 Randy "Villadelfia" Thiemann <uselinuxnow@gmail.com>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not
 *   claim that you wrote the original software. If you use this software
 *   in a product, an acknowledgment in the product documentation would be
 *   appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not be
 *   misrepresented as being the original software.
 *
 *   3. This notice may not be removed or altered from any source
 *   distribution.
 *
 */

#include "Decimal.h"
#include "Debug.h"

SYLPH_BEGIN_NAMESPACE

Decimal::Decimal() : data(0u) {}

Decimal::Decimal(sint i) {
    bool negative = i < 0;
    if(negative)
        i = -i;

    if(SYLPH_LIKELY(i <= 0x7FFFFF)) {
        // Branch on best case scenario, less than 23 bits, just store it in data.
        data = i;
        if(negative)
            data |= 0x80000000;
    } else {
        // If the int doesn't fit in 23 bits, shift right until it does, and
        // take the amount of shifts as exponent.
        suint ctr = 0;
        while(i > 0x7FFFFF) {
            ++ctr;
            i >>= 1;
        }
        ctr <<= 23;
        data = ctr | i;
        if(negative)
            data |= 0x80000000;
    }
}

Decimal::Decimal(sint i, suint r) {
    (void) i;
    (void) r;
    SYLPH_STUB;
}

Decimal::Decimal(float f) {
    (void) f;
    SYLPH_STUB;
}

Decimal::Decimal(double d) {
    (void) d;
    SYLPH_STUB;
}

SYLPH_END_NAMESPACE

// vim: syntax=cpp11:ts=4:sts=4:sw=4:sta:et:tw=80:nobk
