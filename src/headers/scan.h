#ifndef __SCAN_H_
#define __SCAN_H_

#include "globals.h"
#include "utils.h"

#define MAXTOKENLENGTH 40

extern char tokenString[MAXTOKENLENGTH + 1];

TokenType getToken ();

bool openFile (const char *);

void closeFile (void);

int getLineNumber ();

void printToken (TokenType);

#endif