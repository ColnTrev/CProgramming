#include <string.h>
#include <stdio.h>
#include <stdlib.h>
typedef char* string;
typedef struct{
    string item;
} Package;

int main(){
    string words = "Hello I am Collin";
    void* thing = "Hello!";
    Package stuff = {.item = words};
    puts(stuff.item);
    puts(thing);
}