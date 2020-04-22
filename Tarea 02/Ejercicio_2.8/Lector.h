#ifndef LECTORH
#define LECTORH

#include <stdlib.h>
#include <stdio.h>

extern FILE *file;
extern char delimiter;

void init();
void SetDilimiter(char Delimiter);
int NextInt();
long NextLong();
float NextFloat();
double NextDouble();
char * Next();

#endif // LECTORH
