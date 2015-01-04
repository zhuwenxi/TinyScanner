#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#define bool int
#define false 0
#define true 1

extern bool isDebug;
void log (char *, char *);
void pause(bool);

#endif