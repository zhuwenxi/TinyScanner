#include "headers/globals.h"
#include "headers/utils.h"
#include "headers/scan.h"
#include <stdbool.h>
#include <stdio.h>
#include <malloc.h>

FILE *fp = NULL;

typedef enum
{
	START,
	INCOMMENT,
	INNUM,
	INID,
	INASSIGN,
	DONE, 
	ERROR,
} State;

// reserved words list:
const char rw[][10] = {"if", "then", "else", "end", "repeat", "until", "read", "write"};

char tokenString[MAXTOKENLENGTH + 1] = "";

int lineNumber = 0;

int flag = 0;

char *line = NULL;

bool outputByLine = true;

bool endOfFile = false;

// function declaration:
char getNextChar (void);

void ungetNextChar (void);

char getNextCharByLine();

void ungetNextCharByLine();

char* getNextLine();

void handleINCOMMENT (char, State *, TokenType *);

void handleSTART (char, State *, TokenType *);

void handleINNUM (char, State *, TokenType *);

void handleINID (char, State *, TokenType *);

void handleINASSIGN (char, State *, TokenType *);

void updateTokenString (char);

void resetTokenString (void);

bool isDigit (char);

bool isLetter (char);

bool isWhitespace (char);

bool isReservedWord (char *);

TokenType getToken ()
{
	resetTokenString();

	State state = START;
	char ch = '\0';
	TokenType type = UNKNOWN;

	while (state != DONE && state != ERROR) {

		ch = getNextChar();
		// printf("current state: %d\n", state);
		// printf("current ch:%c, ASC2 CODE:%d\n", ch, ch);
		// printf("===============================\n\n");
		switch (state) {
			case START: 
				handleSTART(ch, &state, &type);
				break;
			case INCOMMENT:
				handleINCOMMENT(ch, &state, &type);
				break;
			case INNUM:
				handleINNUM(ch, &state, &type);
				break;
			case INID:
				handleINID(ch, &state, &type);
				break;
			case INASSIGN:
				handleINASSIGN(ch, &state, &type);
				break;
			default:
				state = ERROR;
				printf("ERROR STATE.");
				break;
		}
		
	}

	if (state == DONE) {
		return type;
	} else {
		return UNKNOWN;
	}

}

bool openFile (const char *fileSrc)
{
	fp = fopen(fileSrc, "r");

	if (fp == NULL) {
		// printf("fail!\n");
		return false;
	} else {
		// printf("success!\n");
		return true;
	}
}

void closeFile ()
{
	if (fp != NULL) {
		fclose(fp);
	}
}

char getNextChar () 
{	
	if (outputByLine) 
	{
		return getNextCharByLine();
	}
	else 
	{
		return (char)(fgetc(fp));
	}
	
}

void ungetNextChar () {
	if (outputByLine)
	{
		ungetNextCharByLine();
	}
	else 
	{
		if (!feof(fp)) {
			fseek(fp, -1L, 1);
		}
	}
}

char getNextCharByLine() 
{	
	if (line == NULL)
	{
		line = getNextLine();
		lineNumber++;
		printf("%d:  %s", lineNumber, line);
	}
	
	if (line[flag] == '\0')
	{
		if (feof(fp))
		{	
			// printf("==============================\n");
			// printf("line:%s\n", line);
			// printf("return EOF.\n");
			// printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
			endOfFile = true;
			return EOF;
		}
		else 
		{	
			flag = 0;
			line = getNextLine();
			lineNumber ++;

			// if (lineNumber == 14)
			// {
			// 	printf("first letter of line 14:%d\n", line[0]);
			// }

			
			printf("%d:  %s", lineNumber, line);

			if (feof(fp)){
				printf("\n");
			}
			// printf("call self here.\n");
			return getNextCharByLine();
		}
	} 
	else 
	{	
		// printf("return:%c\n", line[flag]);
		return line[flag++];
	}
}

void ungetNextCharByLine() 
{
	if (endOfFile == false)
	{
		flag--;
	}
}

char *getNextLine ()
{	
	char *line = (char *)malloc(1024 * sizeof(char));

	fgets(line, 1024 * sizeof(char), fp);
	
 	if (feof(fp))
 	{
 		// line[0] = '\0';
 	}
 	
	return line;
}

void handleSTART (char ch, State *sp, TokenType *tp) 
{	
	if (isWhitespace(ch)) {
		// "whitespace", do nothing.
	} else if (ch == '{') {
		// "{", entering a comment.
		*sp = INCOMMENT;
	} else if (isLetter(ch)) {
		// "indentifier"
		*sp = INID;
		updateTokenString(ch);
	} else if (isDigit(ch)) {
		// "digit"
		*sp = INNUM;
		updateTokenString(ch);
	} else if (ch == ':') {
		*sp = INASSIGN;
		updateTokenString(ch);
	} else {
		// Handle special symbols.
		switch (ch) {
			case '+':
				*tp = PLUS;
				break;
			case '-':
				*tp = MINUS;
				break;
			case '*':
				*tp = MUTI;
				break;
			case '/':
				*tp = DIVIDE;
				break;
			case '=':
				*tp = EQUAL;
				break;
			case '<':
				*tp = LESSTHAN;
				break;
			case '(':
				*tp = LEFTBRACKET;
				break;
			case ')':
				*tp = RIGHTBRACKET;
				break;
			case ';':
				*tp = SEMICOLON;
				break;
			case EOF:
				*tp = ENDFILE;
				break;
			default:
				*tp = UNKNOWN;
				break;
		}
		*sp = DONE;

		updateTokenString(ch);

		if (*tp == ENDFILE)
		{
			resetTokenString();
			strcpy(tokenString, "EOF");
		}
	}

	
}

void handleINCOMMENT (char ch, State *sp, TokenType *tp) 
{
	if (ch == '}') {
		*sp = START;
	}
}

void handleINNUM (char ch, State *sp, TokenType *tp) 
{
	if (isDigit(ch)) {
		updateTokenString(ch);
	} else {
		*sp = DONE;
		*tp = NUM;
		ungetNextChar();
	}
}

void handleINID (char ch, State *sp, TokenType *tp) 
{
	if (isLetter(ch)) 
	{
		updateTokenString(ch);
	} else {
		*sp = DONE;
	
		if (!isReservedWord(tokenString)) {
			*tp = ID;
		} else {
			// Hack here, should be reserved word
			*tp = IF;
		}
		
		ungetNextChar();
	}
}

void handleINASSIGN (char ch, State *sp, TokenType *tp) 
{
	if (ch == '=')
	{
		*sp = DONE;
		*tp = ASSIGN;
		updateTokenString(ch);
	} else {
		*sp = DONE;
		*tp = ENDFILE;
		ungetNextChar();
	}
}

void updateTokenString (char ch) 
{
	int length = strlen(tokenString);

	if (length < 40) {
		tokenString[length] = ch;
	}
}

void resetTokenString () 
{
	int length = strlen(tokenString);
	int i;

	for (i = 0 ; i < length; i ++)
	{
		tokenString[i] = '\0';
	}
}
  
bool isDigit (char ch)
{
	return ch >= '0' && ch <= '9';
}

bool isLetter (char ch)
{
	return ch >= 65 && ch <= 122 ;
}

bool isWhitespace (char ch)
{
	// return true if ch "newline", "space", "horizontal tab" or "vertical tab".
	return ch == 10 || ch == 32 || ch == 0 || ch == 11;
}

int getLineNumber ()
{
	return lineNumber;
}

void printToken(TokenType token)
{
	switch (token)
	{
		case ID:
			printf("    %d:  ID, name=%s\n", lineNumber, tokenString);
			break;
		case NUM:
			printf("    %d:  NUM, val=%s\n", lineNumber, tokenString);
			break;
		case PLUS:
		case MINUS:				// 9
		case MUTI:				// 10
		case DIVIDE:			// 11
		case SLASH:				// 12
		case EQUAL:				// 13
		case LESSTHAN:			// 14
		case LEFTBRACKET:		// 15
		case RIGHTBRACKET:		// 16
		case SEMICOLON:			// 17
		case ASSIGN:			// 18
			printf("    %d:  %s\n", lineNumber, tokenString);
			break;
		case ENDFILE:
			printf("    %d:  %s\n", lineNumber, tokenString);
			break;
		case IF:
			printf("    %d:  reserved word: %s\n", lineNumber, tokenString);
			break;
		default:
			printf("Oops!!");
			printf("token type:%d\n", token);
			break;
	}
}

bool isReservedWord (char* token)
{	
	int i;

	for (i = 0 ; i < 8 ; i++)
	{
		const char *reservedWord = rw[i];
		if (strcmp(reservedWord, token) == 0)
		{
			return true;
		}
	}

	return false;
}

// Test
void printLines() {
	while (true) {
		char *str;
		// str = getNextLine();
		str = (char *)malloc(1024);
		fgets(str, 1024, fp);
		if (feof(fp))
		{
			break;
		}
		printf("%s", str);
	}
}


