#include <stdio.h> 
#include <malloc.h> 

extern void ReplaceChar(char symbol, char replacement, char* string); 

void main() 
{ 
  const int MAX_LENGTH = 50; 
  char* buffer = malloc(sizeof(char) * MAX_LENGTH); 
  char symbol = 0; 
  char replacement = 0; 
  
  printf_s("Enter the string: "); 
  
  const char* string = gets_s(buffer, MAX_LENGTH); 
  
  printf_s("Enter symbol and replacement: "); 
  scanf_s("%c", &symbol); 
  scanf_s("%c", &replacement); 
  
  ReplaceChar(symbol, replacement, string); 
  
  printf_s("Result:\n%s", string); 
  
  free(buffer); 
}
