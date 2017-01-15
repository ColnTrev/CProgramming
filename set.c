#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 10
#define SET(type, name, value, size) memset(name, value, size*sizeof(type));
typedef char* string;
int main(){
    int* arr1;
    double* arr2;
    string word;
    arr1 = malloc(SIZE*sizeof(int));
    arr2 = malloc(SIZE*sizeof(double));
    word = malloc(SIZE*sizeof(char));
    SET(int, arr1, 0, SIZE)
    SET(double, arr2, 0, SIZE)
    SET(char, word, 'e', SIZE);
    printf("%d %g\n", arr1[SIZE-1], arr2[SIZE-1]);
    printf("%s \n", word);
    free(arr1);
    free(arr2);
    free(word);
}
