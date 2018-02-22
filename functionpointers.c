//
//  functionpointers.c
//  
//
//  Created by Collin Gordon on 10/8/15.
//
//

#include <stdio.h>
#include <stdlib.h>

int sum(int val1, int val2);
int difference(int val1, int val2);
int product(int val1, int val2);

int main(void)
{
    int (*fptrs[])(int, int) = {sum, difference, product};
    
    printf("The sum is %d\n", (*fptrs[0])(1 , 2));
    printf("The difference is %d\n", (*fptrs[1])(2 , 1));
    printf("The product is %d\n", (*fptrs[2])(1 , 2));
    return EXIT_SUCCESS;
}

int sum(int val1, int val2){return val1 + val2;}
int difference(int val1, int val2){return val1 - val2;}
int product(int val1, int val2){return val1 * val2;}
