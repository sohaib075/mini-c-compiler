/* Test 6: Complex Program */
/* Tests: multiple functions, nested control flow, mixed types, printf */
int factorial(int n) {
    int result = 1;
    int i = 1;
    while (i <= n) {
        result = result * i;
        i = i + 1;
    }
    return result;
}

int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int main() {
    int x = 5;
    int fact = factorial(x);
    int a = 10;
    int b = 25;
    int m = max(a, b);
    printf("Factorial: %d\n", fact);
    printf("Max: %d\n", m);
    return 0;
}
