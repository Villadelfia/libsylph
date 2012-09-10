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

#ifndef SYLPH_CORE_DECIMAL_H_
#define	SYLPH_CORE_DECIMAL_H_

#include "Object.h"
#include "Primitives.h"

SYLPH_BEGIN_NAMESPACE


/**
 * Decimal provides a dec32 according to IEEE754-2008, using the binary
 * integer significand method. In other words a float stored in base 10.
 */
class Decimal {
public:
    /**
     * Default constructor, default stored value is 0.0.
     */
    Decimal();

    /**
     * Constructor that stores an int as dec32.
     * @param i The int to be stored.
     */
    Decimal(sint i);

    /**
     * Constructor that stores the integer part and the real part of a Decimal
     * number as a dec32.
     * @param i The integer part of the decimal number to be stored.
     * @param r The real part of the decimal number to be stored.
     */
    Decimal(sint i, suint r);

    /**
     * Constructor to convert float to dec32.
     * @param f The float to be stored.
     **/
    Decimal(float f);

    /**
     * Constructor to convert double to dec32.
     * @param d The double to be stored.
     */
    Decimal(double d);

    /**
     * Cast to int.
     **/
    operator int() const;

private:
    suint data;    
};

SYLPH_END_NAMESPACE

#endif	/* SYLPH_CORE_DECIMAL_H_ */

// vim: syntax=cpp11:ts=4:sts=4:sw=4:sta:et:tw=80:nobk
