#ifndef __GLOBALS_H_
#define __GLOBALS_H_

typedef enum
{
	// Reserved Word:
	IF,					// 0
	THEN, 				// 1
	ELSE,				// 2
	END,				// 3
	REPEAT,				// 4
	UNTIL,				// 5
	READ,				// 6
	WRITE,				// 7

	// Special Symbol:
	PLUS,				// 8
	MINUS,				// 9
	MUTI,				// 10
	DIVIDE,				// 11
	SLASH,				// 12
	EQUAL,				// 13
	LESSTHAN,			// 14
	LEFTBRACKET,		// 15
	RIGHTBRACKET,		// 16
	SEMICOLON,			// 17
	ASSIGN,				// 18
	ENDFILE,			// 19

	// Number:
	NUM,				// 20

	// Indentifier:
	ID,					// 21

	// Unknown Type:
	UNKNOWN 			// 22
} TokenType;

#endif