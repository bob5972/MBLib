/*
 * mjbutil.h --
 */
 
#ifndef _MJBUTIL_H_201106111445
#define _MJBUTIL_H_201106111445

#ifdef __cplusplus
	extern "C" {
#endif

#define MIN(x, y) ( (x) < (y) ? (x) : (y))
#define MAX(x, y) ( (x) > (y) ? (x) : (y))
#define CLAMP(x, min, max) { x = MIN(x, min); x = MAX(x, max); }

#define ARRAYSIZE(x) ((sizeof x) / (sizeof *x))

#ifdef __cplusplus
	}
#endif 

#endif //_MJBUTIL_H_201106111445
