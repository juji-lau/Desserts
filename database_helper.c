#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

void toLowerCase(char *str)
{
  int string_length = strlen(str);
  for (int i = 0; i < string_length; i++)
  {
    str[i] = tolower(str[i]);
  }
}

int string2summed_int(const char *str)
{
  int string_length = strlen(str);
  int returned_int = 0;
  for (int i = 0; i < string_length; i++)
  {
    returned_int += (int)str[i];
  }
  return returned_int;
}

void print_ascii(char *str){
  int n = strlen(str);
  for(int i = 0; i < n; i++){
    printf("ASCII %d:\n", str[i]);
  }
}

void process_text(char *str){
  int n = strlen(str);
  char *temp_str = malloc(n-1 * sizeof(char));


}