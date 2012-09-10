/*
 * LibSylph Class Library
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

#include "../SylphTest.h"
#include <Sylph/Core/Primitives.h>
#include <Sylph/Core/Decimal.h>
#include <Sylph/Core/Debug.h>

using namespace Sylph;

namespace {

    class TestDecimal : public ::testing::Test {
    };

    TEST_F(TestDecimal, testDefaultCtor) {
        Decimal d;
        EXPECT_EQ(0, (int) d);
    }

    TEST_F(TestDecimal, testSmallInt) {
        Decimal d(0);
        EXPECT_EQ(0, (int) d);

        if(sizeof(int) >= 4){
            for(idx_t i = 0; i < 31; ++i) {
                Decimal d1(1 << i);
                EXPECT_EQ(1 << i, (int) d1);
            }
        }
    }

} // namespace

// vim: syntax=cpp11:ts=4:sts=4:sw=4:sta:et:tw=80:nobk:pa+=,../../src
