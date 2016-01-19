/*
 -----------------------------------------------------
                     utility.cpp                         
                                                          
          Created on: 2014/02/15 11:18:26                  
                                                         
 -----------------------------------------------------
*/

#include "utility.h"

void displayAndExit(const char *message){
    if (message != NULL && (int)strlen(message) != 0)
	printf("%s\n", message);
    exit(0);
}

char *int2str(void *num, unsigned long size, int fixedSize, int extend, bool zero){
    int i = 0, j = 0;
    char result_reverse[10];
    unsigned long temp = *((unsigned long*)num);
    char *result = (char *)malloc(10);
    strcpy(result, "\0");

    if (size <= 0)
	displayAndExit("Size error in int2str");
    
    for (; i < int(size * 2); i++){
	if ((temp % 16) >= 10)
	    result_reverse[size * 2 - i - 1] = (temp % 16) - 10 + 'A';
	else
	    result_reverse[size * 2 - i - 1] = (temp % 16) + '0';
	temp /= 16;
    }
    result_reverse[i] = '\0';
    while(1){
	if (result_reverse[0] != '0')
	    break;
	for (j = 0; j < (int)strlen(result_reverse); j++)
	    result_reverse[j] = result_reverse[j + 1];
    }

    if (fixedSize){
	memset(result, '0', size * 2 - strlen(result_reverse));
	result[size * 2 - strlen(result_reverse)] = '\0';
	strcat(result, result_reverse);
    }
    else{
	if (!extend && result_reverse[0] > '9')
    	    strcpy(result, "0");
    	strcat(result, result_reverse);
    }
    if (extend){
	memset(result, 'F', 8 - strlen(result_reverse));
	result[8 - strlen(result_reverse)] = '\0';
	strcat(result, result_reverse);
    }

    /* for assembler */
    if (zero && result[0] > '9'){
	strcpy(result_reverse, result);
	strcpy(result, "0");
	strcat(result, result_reverse);
    }
    /* ends here*/
    
    return result;
}

char *string2hex(char *str){
    if (str == NULL || strlen(str) <= 1)
	return NULL;

    char *hex = (char*)malloc(strlen(str) / 2);
    unsigned char result;
    memset(hex, 0x0, strlen(str) / 2);

    for (int i = 0; i < (int)strlen(str) / 2; i++){
	result = char2hex(str[i * 2]);
	result = (result << 4) + char2hex(str[i * 2 + 1]);
	hex[i] = result;
    }

    return hex;
}

UINT32 string2bin(char *str, int &size){
    if (str == NULL || strlen(str) < 1)
	return 0;

    char temp[10];
    strcpy(temp, str);

    stripHex(temp);

    int str_size = (int)strlen(temp);
    if (str_size > 8)
	displayAndExit("Size error in string2hex");
    
    UINT32 hex = 0;

    for (int i = 0; i < str_size; i++)
	hex = (hex << 4) + char2hex(temp[i]);

    if (str_size <= 2)
	size = 0;
    else if (str_size <= 4)
	size = 1;
    else if (str_size <= 8)
	size = 2;
    else
	size = -1;

    return hex;
}

void stripHex(char *str){
    int start = 0, end = (int)strlen(str), i, j;
    
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
	start += 2;
    else if (str[end - 1] == 'h' || str[end - 1] == 'H')
	end--;

    while (str[start] == '0')	
	start++;

    for (i = 0, j = start; j < end; j++)
	str[i++] = str[j];
    
    if (i == 0)
	str[i++] = 0x0;
    
    str[i] = '\0';
}

char char2hex(char c){
    if (c >= 'a')
	return c - 'a' + 0xa;
    else if (c >= 'A')
	return c - 'A' + 0xa;
    else
	return c - '0' + 0x0;
}

bool isHex(char *str){
    if (str == NULL)
	return false;

    int start = 0, end = (int)strlen(str), i;
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
	start += 2;
    else if (str[end - 1] == 'h' || str[end - 1] == 'H')
	end--;
    for (i = start; i < end; i++){	
	if (!isxdigit(str[i])){
	    if (i == end - 1 && str[i] == '\n')
		str[end - 1] = '\0';
	    else
		return false;
	}
    }
    return true;
}

bool isEnd(char ch){
    if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\0')
	return true;
    return false;
}

void lower(char *instructions){
    for (int i = 0; i < (int)strlen(instructions); i++)
	if (isupper(instructions[i]))
	    instructions[i] = tolower(instructions[i]);
}
