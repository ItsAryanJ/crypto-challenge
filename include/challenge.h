#ifndef CHALLENGE_H
#define CHALLENGE_H

#define MAX_TEXT 256
#define MAX_KEY 64

typedef struct {
    int cipherId;
    int isEncrypt;
    char plaintext[MAX_TEXT];
    char key[MAX_KEY];
    char challengeText[MAX_TEXT];
    char expectedAnswer[MAX_TEXT];
    int timeLimit;
} Challenge;

void configure_challenge();
void play_challenge_mode();

#endif
