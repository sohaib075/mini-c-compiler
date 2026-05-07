/* Test 20: Preprocessor + All Features Combined */
#include <stdio.h>

int abs_val(int n) {
    if (n < 0) {
        return -n;
    }
    return n;
}

int main() {
    int x = -7;
    int a = abs_val(x);
    printf("Absolute: %d\n", a);
    
    int sum = 0;
    int i;
    for (i = 0; i < 10; i = i + 1) {
        if (i % 2 == 0) {
            sum = sum + i;
        }
    }
    printf("Sum of evens: %d\n", sum);
    return 0;
}
