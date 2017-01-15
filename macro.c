#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//macro here
#define arrset(type, name, size) type name[size]; memset(name, 0, size*sizeof(type));

void* init(){
    arrset(int, numbers, 10)
    return (void*)numbers;
}
int main(){
     int* things = init();
     printf("%d\n", things[0]);
}