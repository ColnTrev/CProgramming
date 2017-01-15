#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define forloop(i, loopmax,...) for(int i=0; i < loopmax; i++){__VA_ARGS__}
double sum_array(double in[]){
    double out = 0;
    for(int i = 0; !isnan(in[i]); i++) out += in[i];
    return out;
}
#define sum(...)sum_array((double[]){__VA_ARGS__, NAN})
typedef struct{
    int key;
    int value;
} Dict;
Dict info(int k, int v);
int main(){
    Dict item = info(1,3);
    Dict item2 = {.value = 4, .key = 2};
    printf("Key: %d Value: %d\n", item.key, item.value);
    printf("Key: %d Value: %d\n", item2.key, item2.value);
    int sum = 0;
    double two_and_two = sum(2, 2);
    forloop(i,10,
        sum+=i;
        printf("sum to %i: %i\n", i, sum);
    )
    printf("2+2 = %g\n", two_and_two);
    printf("(2+2) * 2 = %g\n", sum(two_and_two, two_and_two));
}

Dict info(int k, int v){
    return (Dict){k,v};
}