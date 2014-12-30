#include <stdio.h>
#include "headers/utils.h"

bool isDebug = true;

void log (char *str, char *ref)
{
	if (!isDebug) {
		return ;
	}

	printf(str, ref);
}

void pause (bool shouldPause) {
	if (!isDebug) {
		return ;
	}

	if (shouldPause) {
		system("Pause");
	}
}