#ifndef HILL_H
#define HILL_H

char* hill_encrypt(const char* plaintext, int K[2][2]);
char* hill_decrypt(const char* ciphertext, int K[2][2]);
int is_invertible_2x2(int K[2][2]);

#endif
