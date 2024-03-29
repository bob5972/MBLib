/*
 * MBOpt.h -- part of MBLib
 *
 * Copyright (c) 2020-2021 Michael Banack <github@banack.net>
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

#ifndef MBOpt_H_202006121333
#define MBOpt_H_202006121333

#include "MBTypes.h"

#ifdef __cplusplus
	extern "C" {
#endif

typedef struct MBOption {
    const char *shortOpt;
    const char *longOpt;
    bool extraArg;
    const char *helpText;
} MBOption;

const char *MBOpt_GetProgramName(void);
const char *MBOpt_GetProgramVersion(void);

void MBOpt_SetProgram(const char *name, const char *version);
void MBOpt_LoadOptions(const char *cmd, MBOption *opts, int numOpts);
void MBOpt_Init(int argc, char **argv);
void MBOpt_Exit(void);

void MBOpt_PrintMBLibVersion(void);
void MBOpt_PrintHelpText(void);

const char *MBOpt_GetCmd(void);

bool MBOpt_IsValid(const char *option);
bool MBOpt_IsPresent(const char *option);
const char *MBOpt_GetCStr(const char *option);
int MBOpt_GetInt(const char *option);
int64 MBOpt_GetInt64(const char *option);
uint MBOpt_GetUint(const char *option);
uint64 MBOpt_GetUint64(const char *option);
bool MBOpt_GetBool(const char *option);
float MBOpt_GetFloat(const char *option);

#ifdef __cplusplus
	}
#endif

#endif //MBOpt_H_202006121333
