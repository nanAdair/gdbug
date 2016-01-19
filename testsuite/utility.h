/* @(#)utility.h
 */

#ifndef _UTILITY_H
#define _UTILITY_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
//#include <iostream>

typedef unsigned char   UINT8;
typedef unsigned short  UINT16;
typedef unsigned int    UINT32;
typedef char            INT8;
typedef short           INT16;
typedef int             INT32;
typedef long long       INT64;

void displayAndExit(const char *message);
char *int2str(void *num, unsigned long size, int fixedSize, int extend, bool zero = true);
char *string2hex(char *str);
UINT32 string2bin(char *str, int &size);
char char2hex(char c);
void stripHex(char *str);
bool isHex(char *str);
bool isEnd(char ch);
void lower(char *instructions);
void errorAndExit(int line_number, char *instruction, char *err_msg, int start_point, int end_point, UINT8 err_type);
void errorAndExit(char *err_msg);

#endif /* _UTILITY_H */



