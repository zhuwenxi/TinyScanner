#include <stdio.h>
#include "headers/globals.h"
#include "headers/scan.h"
#include <malloc.h>

int main ()
{
	const char *sampleSrc = "C:\\Users\\wenxizhu\\Desktop\\workspace\\TinyScanner\\samples\\sample.tny";
	bool success = openFile(sampleSrc);

	if (success) {
		TokenType t = getToken();
		while (t != ENDFILE)
		{
			// printf("    %d: %s\n", getLineNumber(), tokenString);
			printToken(t);
			t = getToken();
		}

		printToken(t);

	} else {
		printf("Oops!!");
	}

	// Do some test here.
	system("PAUSE");
}	