#include "Lector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *file;
char delimiter;

long IntigerParse(char *chain){
    int size = strlen(chain);
    long r = 0;
    long reg = 1;
    for(int i = size-1; i>=0; i--){
        char c = *(chain + i);
        if(c >= '0' && c <= '9'){
            r += (((long)(c - '0'))*reg);
            reg*=10;
        } else if (c == '-'){
            r *= -1;
        }
    }
    return r;
}

void init(FILE *File){
    file = File;
    rewind(file);
    delimiter = ' ';
}

void SetDilimiter(char Delimiter){
    delimiter = Delimiter;
}

int NextInt(){
    char *string = Next();
    int r = IntigerParse(string);
    free(string);
    return (int)r;
}

long NextLong(){
    char *string = Next();
    long r = IntigerParse(string);
    free(string);
    return r;
}

float NextFloat(){
    float number = 0.0;
    char * string = Next();
    int size = strlen(string), pointPos = size, reg;
    for(int i = 0; i<size; i++){
        if(*(string + i) == '.'){
            pointPos = i;
            break;
        }
    }

    reg = 1;
    for(int i = pointPos - 1; i>=0; i--){
        char c = *(string + i);
        if(c >= '0' && c<= '9'){
            number += (float)(((float)(c - '0'))*reg);
            reg *= 10;
        } else if (c == '-'){
            number *= -1;
        }
    }

    reg = 10;
    for(int i = pointPos + 1; i<size; i++){
        char c = *(string + i);
        if(c >= '0' && c<= '9'){
            number += (float)(((float)(c - '0'))/reg);
            reg *= 10;
        }
    }

    return number;
}

double NextDouble(){
    double number = 0.0;
    char * string = Next();
    int size = strlen(string), pointPos = size, reg;
    for(int i = 0; i<size; i++){
        if(*(string + i) == '.'){
            pointPos = i;
            break;
        }
    }

    reg = 1;
    for(int i = pointPos - 1; i>=0; i--){
        char c = *(string + i);
        if(c >= '0' && c<= '9'){
            number += (float)(((float)(c - '0'))*reg);
            reg *= 10;
        } else if (c == '-'){
            number *= -1;
        }
    }

    reg = 10;
    for(int i = pointPos + 1; i<size; i++){
        char c = *(string + i);
        if(c >= '0' && c<= '9'){
            number += (float)(((float)(c - '0'))/reg);
            reg *= 10;
        }
    }

    return number;
}

char * Next(){
    char *c = malloc(100);
    char *reg = malloc(2);
    int cont = 1;
    *c = '\0';
    *reg = '\0';
    *(reg + 1) = '\0';

    while(cont == 1){
        int r;
        do{
            r = fread(reg, 1, 1, file);
            if(feof(file) != 0){
                *reg = ' ';
                break;
            }
        }while(r==0);

        if(*reg != ' ' && *reg != delimiter && *reg != '\n'){
            strcat(c, reg);
        } else {
            strcat(c, "\0");
            cont = 0;
        }
    }

    return c;
}
