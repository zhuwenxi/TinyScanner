#include "headers/globals.h"
#include "headers/utils.h"
#include "headers/scan.h"
#include <stdbool.h>
#include <stdio.h>

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

char tokenString[MAXTOKENLENGTH + 1] = "";
// function declaration:
char getNextChar (void);

void ungetNextChar (void);

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

char getNextChar () 
{
	return (char)(fgetc(fp));
}

void ungetNextChar () {
	if (!feof(fp)) {
		fseek(fp, -1L, 1);
	}
}

int counter = 0;

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
		*tp = ID;
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


