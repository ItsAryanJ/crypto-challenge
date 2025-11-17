#ifndef UTILS_H
#define UTILS_H

void clear_input_buffer();
void press_enter();
void str_upper(char* s);
void remove_nonalpha(char* dest, const char* src);
int random_int(int min, int max);
void trim_newline(char* s);

#endif
