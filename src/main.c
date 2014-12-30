#include <stdio.h>
#include "headers/globals.h"
#include "headers/scan.h"

int main ()
{
	const char *sampleSrc = "C:\\Users\\wenxizhu\\Desktop\\workspace\\Projects\\TinyScanner\\samples\\sample.tny";
	bool success = openFile(sampleSrc);

	if (success) {
		TokenType t = getToken();
		while (t != ENDFILE)
		{
			printf("%s\n", tokenString);
			// printf("%d\n", t);
			t = getToken();
		}

		// int i;
		// for (i = 0 ; i < 100 ; i ++)
		// {
		// 	TokenType t = getToken();
		// 	printf("%s\n", tokenString);
		// 	printf("%d\n", t);
		// }
	} else {
		printf("Oops!!");
	}
	system("PAUSE");
}