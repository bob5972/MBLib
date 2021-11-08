/*
 * main.c -- part of MBLib
 *
 * This is a sample main file intended as a template for starting a project
 * using MBLib, and demonstrates how to link into the build-system.
 */

#include <stdio.h>

#include "MBConfig.h"

int main()
{
    printf("Hello World!\n");
    printf("\n");
    printf("MBLib sample project\n");
    printf("debug=%d, devel=%d\n", mb_debug, mb_devel);
    return 0;
}

