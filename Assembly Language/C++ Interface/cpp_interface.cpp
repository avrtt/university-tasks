#include <stdio.h>
#include <malloc.h>
#include <string.h>
 
extern void FormatString(char* string, int length);
 
void main() 
{
	const int MAX_LENGTH = 50;
	char* buffer = malloc(sizeof(char) * MAX_LENGTH);
 
	printf_s("%s", "Enter the string: ");
	const char* string = gets_s(buffer, MAX_LENGTH);
 
	int length = strlen(string);
 
	FormatString(string, length);
 
	printf_s("Compressed string:\n%s", string);
 
	free(buffer);
}
