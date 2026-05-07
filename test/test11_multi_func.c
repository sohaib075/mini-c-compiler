/* Test 11: Multiple Functions with Calls */
/* Tests: multi-function, return value, calling between functions */
int square(int x) {
    return x * x;
}

int add(int a, int b) {
    return a + b;
}

int compute(int x, int y) {
    int s = square(x);
    int sum = add(s, y);
    return sum;
}

int main() {
    int result = compute(3, 4);
    printf("Result: %d\n", result);
    return 0;
}
