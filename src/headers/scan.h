#ifndef __SCAN_H_
#define __SCAN_H_

#include "globals.h"
#include <stdbool.h>

#define MAXTOKENLENGTH 40

extern char tokenString[MAXTOKENLENGTH + 1];
TokenType getToken ();
bool openFile (const char *);

#endif