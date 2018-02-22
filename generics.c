#include <stdio.h>
#include <stdlib.h>
#define ADD1(x) _Generic((x), int: x + 1, double: x + 1.0, default: 0)
#define PRINTFORM(x) _Generic((x), int: "%d", double: "%.2f")
#define PRINT(x) printf(PRINTFORM(x), x), printf("\n")

int main(void)
{
    int num1    = 3;
    double num2 = 4.5;
    PRINT(ADD1(num1));
    PRINT(ADD1(num2));
    return EXIT_SUCCESS;
}