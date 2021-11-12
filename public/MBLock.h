/*
 * MBLock.h -- part of MBLib
 *
 * Copyright (c) 2021 Michael Banack <github@banack.net>
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

#ifndef _MBLOCK_H_20211110
#define _MBLOCK_H_20211110

#ifndef MB_HAS_SDL2
#error MBLock.h requires SDL2
#endif

#include <SDL2/SDL_mutex.h>
#include <SDL2/SDL_thread.h>

#include "MBBasic.h"
#include "MBAssert.h"

#define MB_THREAD_ID_INVALID 0

typedef struct MBLock {
    SDL_mutex *sdlMutex;
    SDL_threadID thread;
} MBLock;

static INLINE void
MBLock_Create(MBLock *lock)
{
    ASSERT(lock != NULL);
    lock->sdlMutex = SDL_CreateMutex();
    VERIFY(lock->sdlMutex != NULL);

    if (mb_debug) {
        lock->thread = MB_THREAD_ID_INVALID;
    }
}

static INLINE
void MBLock_Destroy(MBLock *lock)
{
    ASSERT(lock != NULL);
    ASSERT(lock->sdlMutex != NULL);

    if (mb_debug) {
        ASSERT(lock->thread == MB_THREAD_ID_INVALID);
    }

    SDL_DestroyMutex(lock->sdlMutex);
    lock->sdlMutex = NULL;
}

static INLINE bool
MBLock_IsLocked(MBLock *lock)
{
    ASSERT(lock != NULL);

    if (!mb_debug) {
        NOT_IMPLEMENTED();
    }

    if (lock->thread == MB_THREAD_ID_INVALID) {
        return FALSE;
    }

    return SDL_GetThreadID(NULL) == lock->thread;
}

static INLINE void
MBLock_Lock(MBLock *lock)
{
    ASSERT(lock != NULL);
    ASSERT(!MBLock_IsLocked(lock));
    int ret = SDL_LockMutex(lock->sdlMutex);
    ASSERT(ret == 0);

    if (mb_debug) {
        ASSERT(lock->thread == MB_THREAD_ID_INVALID);
        lock->thread = SDL_GetThreadID(NULL);
        ASSERT(lock->thread != MB_THREAD_ID_INVALID);
    }
}

static INLINE void
MBLock_Unlock(MBLock *lock)
{
    ASSERT(lock != NULL);
    ASSERT(lock->sdlMutex != NULL);

    if (mb_debug) {
        ASSERT(MBLock_IsLocked(lock));
        lock->thread = MB_THREAD_ID_INVALID;
    }

    int ret = SDL_UnlockMutex(lock->sdlMutex);
    ASSERT(ret == 0);
}

#endif // _MBLOCK_H_20211110
