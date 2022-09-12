/*
 * test.cpp -- part of MBLib
 *
 * Copyright (c) 2015-2021 Michael Banack <github@banack.net>
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

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "MBUnitTest.h"
#include "MBOpt.h"
#include "MBBasic.h"

int main(int argc, char *argv[])
{
    bool benchmark;

    MBOption opts[] = {
        { "-h", "--help",      FALSE, "Print the help text" },
        { "-v", "--version",   FALSE, "Print the help text" },
        { "-b", "--benchmark", FALSE, "Run the benchmark"   },
        { "-t", "--tests",     FALSE, "Run the unit tests"  },
    };

    MBOpt_LoadOptions(NULL, opts, ARRAYSIZE(opts));
    MBOpt_Init(argc, argv);

    if (MBOpt_IsPresent("help") || MBOpt_IsPresent("version")) {
        MBOpt_PrintHelpText();
        exit(0);
    }

#ifdef BENCHMARK
    benchmark = TRUE;
#else
    benchmark = FALSE;
#endif

    if (MBOpt_IsPresent("benchmark")) {
        benchmark = TRUE;
    } else if (MBOpt_IsPresent("tests")) {
        benchmark = FALSE;
    }

    if (benchmark) {
        MBUnitTest_RunBenchmark();
    } else {
        MBUnitTest_RunTests();
    }

    MBOpt_Exit();

    printf("Done.\n");
    return 0;
}
