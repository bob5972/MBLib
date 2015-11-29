/*
 * This file is part of MBLib.
 * Copyright (c) 2015 Michael Banack <bob5972@banack.net>
 *
 * MBLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MBLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MBLib.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * MBVector.c --
 */

#include "MBVector.h"

void MBVector_EnsureCapacity(MBVector *vector, int capacity)
{
    int newCap;
    ASSERT(vector->capacity > 0);
    ASSERT(vector->size >= 0);
    ASSERT(capacity >= 0);
    ASSERT(vector->itemSize > 0);

    if (vector->capacity >= capacity) {
        return;
    }

    newCap = vector->capacity;
    while (newCap < capacity) {
        newCap *= 2;
    }
    ASSERT(newCap > vector->capacity);

    vector->items = realloc(vector->items, vector->itemSize * newCap);
}
