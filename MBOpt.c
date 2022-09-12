/*
 * MBOpt.c -- part of MBLib
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

#include <stdlib.h>

#include "MBOpt.h"
#include "MBTypes.h"
#include "MBAssert.h"
#include "MBRegistry.h"
#include "MBVector.h"

typedef struct MBOptionEntry {
    const char *cmd;
    MBOption opt;
    bool present;
    const char *value;
} MBOptionEntry;

DECLARE_CMBVECTOR_TYPE(MBOptionEntry, CMBOptVec);

typedef struct MBOptGlobalData {
    bool initialized;
    const char *arg0;
    const char *argCmd;
    const char *programVersion;
    const char *programName;

    MBRegistry *mreg;
    bool entriesInitialized;
    CMBOptVec entries;
    bool cmdsInitialized;
    CMBCStrVec cmds;
} MBOptGlobalData;

static MBOptGlobalData mbopt;

static void MBOptInitEntries(int capacity)
{
    if (!mbopt.entriesInitialized) {
        CMBOptVec_Create(&mbopt.entries, 0, capacity);
        mbopt.entriesInitialized = TRUE;
    } else {
        int size = CMBOptVec_Size(&mbopt.entries);
        CMBOptVec_EnsureCapacity(&mbopt.entries, capacity + size);
    }
}

static void MBOptInitCmds(int capacity)
{
    if (!mbopt.cmdsInitialized) {
        CMBCStrVec_Create(&mbopt.cmds, 0, capacity);
        mbopt.cmdsInitialized = TRUE;
    } else {
        int size = CMBCStrVec_Size(&mbopt.cmds);
        CMBCStrVec_EnsureCapacity(&mbopt.cmds, capacity + size);
    }
}

void MBOpt_SetProgram(const char *name, const char *version)
{
    mbopt.programVersion = version;
    mbopt.programName = name;
}

const char *MBOpt_GetProgramName(void)
{
    if (mbopt.programName != NULL) {
        return mbopt.programName;
    } else if (mbopt.arg0 != NULL) {
        return mbopt.arg0;
    } else {
        return NULL;
    }
}

const char *MBOpt_GetProgramVersion(void)
{
    if (mbopt.programVersion != NULL) {
        return mbopt.programVersion;
    } else {
        return NULL;
    }
}

const char *MBOpt_GetCmd(void)
{
    return mbopt.argCmd;
}


void MBOpt_LoadOptions(const char *cmd, MBOption *opts, int numOpts)
{
    bool addCmd = TRUE;
    ASSERT(!mbopt.initialized);
    ASSERT(numOpts >= 0);
    ASSERT(numOpts < MAX_INT32);

    /*
     * Don't allow commands to start with a dash to distinguish them from
     * options.
     */
    ASSERT(cmd == NULL || cmd[0] != '-');

    MBOptInitEntries(numOpts);
    MBOptInitCmds(1);

    if (cmd != NULL) {
        for (int i = 0; i < CMBCStrVec_Size(&mbopt.cmds); i++) {
            const char *s = CMBCStrVec_GetValue(&mbopt.cmds, i);
            ASSERT(cmd != NULL);
            ASSERT(s != NULL);
            if (strcmp(cmd, s) == 0) {
                addCmd = FALSE;
                break;
            }
        }
    } else {
        addCmd = FALSE;
    }

    if (addCmd) {
        CMBCStrVec_AppendValue(&mbopt.cmds, cmd);
    }

    for (uint32 i = 0; i < numOpts; i++) {
        CMBOptVec_Grow(&mbopt.entries);
        MBOptionEntry *e = CMBOptVec_GetLastPtr(&mbopt.entries);

        e->opt = opts[i];
        e->present = FALSE;
        e->cmd = cmd;
        e->value = NULL;

        ASSERT(e->opt.shortOpt == NULL || e->opt.shortOpt[0] == '-');
        ASSERT(e->opt.longOpt != NULL);
        ASSERT(e->opt.longOpt[0] == '-');
        ASSERT(e->opt.longOpt[1] == '-');
    }
}

void MBOpt_Init(int argc, char **argv)
{
    int argStart = 0;

    ASSERT(!mbopt.initialized);

    MBOption default_opts[] = {
        { "-h", "--help",      FALSE, "Print the help text"           },
        { "-v", "--version",   FALSE, "Print the version information" },
    };
    MBOpt_LoadOptions(NULL, default_opts, ARRAYSIZE(default_opts));

    MBOptInitEntries(0);
    MBOptInitCmds(0);

    mbopt.mreg = MBRegistry_Alloc();

    ASSERT(argc >= 1);
    mbopt.arg0 = argv[0];
    argStart++;

    mbopt.initialized = TRUE;
    if (argc <= 1) {
        return;
    }

    for (int i = 0; i < CMBCStrVec_Size(&mbopt.cmds); i++) {
        const char *c = CMBCStrVec_GetValue(&mbopt.cmds, i);
        ASSERT(argv[1] != NULL);
        if (c != NULL && strcmp(c, argv[1]) == 0) {
            mbopt.argCmd = argv[1];
            argStart++;
            break;
        }
    }

    ASSERT(argStart == 1 || argStart == 2);
    for (uint32 i = argStart; i < argc; i++) {
        for (uint32 o = 0; o < CMBOptVec_Size(&mbopt.entries); o++) {
            MBOptionEntry *e = CMBOptVec_GetPtr(&mbopt.entries, o);

            if (e->cmd != NULL && mbopt.argCmd == NULL) {
                continue;
            } else if (e->cmd != NULL && mbopt.argCmd != NULL &&
                       strcmp(e->cmd, mbopt.argCmd) != 0) {
                    continue;
            }

            ASSERT(e->cmd == NULL || strcmp(e->cmd, mbopt.argCmd) == 0);

            if ((e->opt.shortOpt != NULL &&
                 strcmp(argv[i], e->opt.shortOpt) == 0) ||
                (e->opt.longOpt != NULL &&
                 strcmp(argv[i], e->opt.longOpt) == 0)) {
                e->present = TRUE;
                e->value = "TRUE";

                if (e->opt.extraArg) {
                    if (i + 1 < argc) {
                        e->value = argv[i+1];
                    } else {
                        PANIC("Option %s expected an argument\n", argv[i]);
                    }
                }

                MBRegistry_PutCopy(mbopt.mreg, &e->opt.longOpt[2], e->value);
                break;
            }
        }
    }


    if (MBOpt_IsPresent("version")) {
        MBOpt_PrintMBLibVersion();
        exit(0);
    }
    if (MBOpt_IsPresent("help")) {
        MBOpt_PrintHelpText();
        exit(0);
    }
}

void MBOpt_Exit(void)
{
    ASSERT(mbopt.initialized);
    MBRegistry_Free(mbopt.mreg);
    mbopt.mreg = NULL;

    ASSERT(mbopt.entriesInitialized);
    CMBOptVec_Destroy(&mbopt.entries);
    mbopt.entriesInitialized = FALSE;

    ASSERT(mbopt.cmdsInitialized);
    CMBCStrVec_Destroy(&mbopt.cmds);
    mbopt.cmdsInitialized = FALSE;

    mbopt.initialized = FALSE;
}

void MBOpt_PrintMBLibVersion(void)
{
    Warning("\n");
    if (MBOpt_GetProgramName() != NULL) {
        if (MBOpt_GetProgramVersion() != NULL) {
            Warning("%s version %s\n", MBOpt_GetProgramName(),
                    MBOpt_GetProgramVersion());
        } else {
            Warning("%s\n", MBOpt_GetProgramName());
        }
    }

    Warning("\tMBLib version %s\n", MBLIB_VERSION_STRING);
    Warning("\tMB_DEBUG=%d, MB_DEVEL=%d\n", mb_debug, mb_devel);
    Warning("\tMB_HAS_SDL2=%d\n", mb_has_sdl2);
    Warning("\n");
}

void MBOpt_PrintHelpText(void)
{
    int cmdLength = 0;
    int optLength = 0;
    MBOpt_PrintMBLibVersion();

    if (CMBCStrVec_Size(&mbopt.cmds) > 1) {
        Warning("Usage:\n");
        Warning("\t%s [cmd] [options]\n", MBOpt_GetProgramName());

        Warning(" Commands:\n\t");
        for (int c = 0; c < CMBCStrVec_Size(&mbopt.cmds); c++) {
            const char *cmd = CMBCStrVec_GetValue(&mbopt.cmds, c);
            cmdLength = MAX(cmdLength, strlen(cmd));
            Warning("%s, ", cmd);
        }
        Warning("\n");
    } else if (MBOpt_GetProgramName() != NULL) {
        Warning("%s Usage:\n", MBOpt_GetProgramName());
    } else {
        Warning(" Usage:\n");
    }

    for (uint32 i = 0; i < CMBOptVec_Size(&mbopt.entries); i++) {
        MBOptionEntry *e = CMBOptVec_GetPtr(&mbopt.entries, i);
        optLength = MAX(optLength, strlen(e->opt.longOpt));
    }

    Warning(" Options:\n", mbopt.arg0);
    for (uint32 i = 0; i < CMBOptVec_Size(&mbopt.entries); i++) {
        MBOptionEntry *e = CMBOptVec_GetPtr(&mbopt.entries, i);
        if (CMBCStrVec_Size(&mbopt.cmds) > 1) {
            Warning(" %*s | %s, %*s: %s\n",
                    cmdLength, e->cmd != NULL ? e->cmd : "",
                    e->opt.shortOpt,
                    optLength, e->opt.longOpt,
                    e->opt.helpText);
        } else {
            ASSERT(strlen(e->opt.longOpt) <= 16);
            Warning("\t| %s, %*s: %s\n",
                    e->opt.shortOpt,
                    optLength, e->opt.longOpt, e->opt.helpText);
        }
    }
}


bool MBOpt_IsValid(const char *option)
{
    if (MBRegistry_ContainsKey(mbopt.mreg, option)) {
        return TRUE;
    }

    for (uint32 i = 0; i < CMBOptVec_Size(&mbopt.entries); i++) {
        MBOptionEntry *e = CMBOptVec_GetPtr(&mbopt.entries, i);

        if (e->cmd != NULL && mbopt.argCmd == NULL) {
            continue;
        } else if (e->cmd != NULL && mbopt.argCmd != NULL &&
                   strcmp(e->cmd, mbopt.argCmd) != 0) {
                continue;
        }

        ASSERT(e->cmd == NULL || strcmp(e->cmd, mbopt.argCmd) == 0);

        if (strcmp(option, &e->opt.longOpt[2]) == 0) {
            return TRUE;
        }
    }

    return FALSE;
}


bool MBOpt_IsPresent(const char *option)
{
    if (MBRegistry_ContainsKey(mbopt.mreg, option)) {
        return TRUE;
    }

    if (mb_debug) {
        if (!MBOpt_IsValid(option)) {
            PANIC("Unknown Option: %s\n", option);
        }
    }

    return FALSE;
}

const char *MBOpt_GetCStr(const char *option)
{
    if (mb_debug) {
        if (!MBOpt_IsValid(option)) {
            PANIC("Unknown Option: %s\n", option);
        }
    }

    return MBRegistry_GetCStr(mbopt.mreg, option);
}

int MBOpt_GetInt(const char *option)
{
    if (mb_debug) {
        if (!MBOpt_IsValid(option)) {
            PANIC("Unknown Option: %s\n", option);
        }
    }

    return MBRegistry_GetInt(mbopt.mreg, option);
}


int64 MBOpt_GetInt64(const char *option)
{
    if (mb_debug) {
        if (!MBOpt_IsValid(option)) {
            PANIC("Unknown Option: %s\n", option);
        }
    }

    return MBRegistry_GetInt64(mbopt.mreg, option);
}

uint MBOpt_GetUint(const char *option)
{
    if (mb_debug) {
        if (!MBOpt_IsValid(option)) {
            PANIC("Unknown Option: %s\n", option);
        }
    }

    return MBRegistry_GetUint(mbopt.mreg, option);
}


uint64 MBOpt_GetUint64(const char *option)
{
    if (mb_debug) {
        if (!MBOpt_IsValid(option)) {
            PANIC("Unknown Option: %s\n", option);
        }
    }

    return MBRegistry_GetUint64(mbopt.mreg, option);
}


bool MBOpt_GetBool(const char *option)
{
    if (mb_debug) {
        if (!MBOpt_IsValid(option)) {
            PANIC("Unknown Option: %s\n", option);
        }
    }

    return MBRegistry_GetBool(mbopt.mreg, option);
}

float MBOpt_GetFloat(const char *option)
{
    if (mb_debug) {
        if (!MBOpt_IsValid(option)) {
            PANIC("Unknown Option: %s\n", option);
        }
    }

    return MBRegistry_GetFloat(mbopt.mreg, option);
}
