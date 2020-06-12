/*
 * MBOpt.c -- part of MBLib
 *
 * Copyright (c) 2020 Michael Banack <github@banack.net>
 *
 * MIT License
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>

#include "MBOpt.h"
#include "mbtypes.h"
#include "mbassert.h"

typedef struct MBOptionValue {
    MBOption opt;
    bool present;
} MBOptionValue;

typedef struct MBOptGlobalData {
    bool initialized;
    const char *arg0;

    MBOptionValue *values;
    uint32 numOpts;
} MBOptGlobalData;

static MBOptGlobalData mbopt;

void MBOpt_Init(MBOption *opts, int numOpts, int argc, char **argv)
{
    ASSERT(!mbopt.initialized);
    ASSERT(numOpts >= 0);
    ASSERT(numOpts < MAX_INT32);

    mbopt.values = malloc(numOpts * sizeof(mbopt.values[0]));
    mbopt.numOpts = numOpts;
    for (uint32 i = 0; i < numOpts; i++) {
        mbopt.values[i].opt = opts[i];
        mbopt.values[i].present = FALSE;

        ASSERT(mbopt.values[i].opt.shortOpt != NULL);
        ASSERT(mbopt.values[i].opt.shortOpt[0] == '-');
        ASSERT(mbopt.values[i].opt.longOpt != NULL);
        ASSERT(mbopt.values[i].opt.longOpt[0] == '-');
        ASSERT(mbopt.values[i].opt.longOpt[1] == '-');
    }

    ASSERT(argc >= 1);
    mbopt.arg0 = argv[0];

    for (uint32 i = 1; i < argc; i++) {
        for (uint32 o = 0; o < numOpts; o++) {
            if (strcmp(argv[i], mbopt.values[o].opt.shortOpt) == 0 ||
                strcmp(argv[i], mbopt.values[o].opt.longOpt) == 0) {
                mbopt.values[o].present = TRUE;
            }
        }
    }

    mbopt.initialized = TRUE;
}

void MBOpt_Exit(void)
{
    ASSERT(mbopt.initialized);
    mbopt.initialized = FALSE;
}


bool MBOpt_IsPresent(const char *option)
{
    for (uint32 i = 0; i < mbopt.numOpts; i++) {
        if (strcmp(option, &mbopt.values[i].opt.longOpt[2]) == 0) {
            return mbopt.values[i].present;
        }
    }

    PANIC("Unknown Option: %s\n", option);
}
