// converts all characters in the string, str to lowercase
void toLowerCase(char *str);
// Returns every character in str to the ascii integer representation and sums them up
int string2summed_int(const char *str);

// Print the ascii value of every element in the string
void print_ascii(char *str);

// Returns a text in a linux-friendly enviornment; removes the return_carraige line, and beginning junk
char *process_text(char *str);