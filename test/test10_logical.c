/* Test 10: Logical and Relational Operators */
/* Tests: &&, ||, !, ==, !=, <, >, <=, >= */
int main() {
    int a = 5;
    int b = 10;
    int c = 5;
    
    int r1 = (a == c);
    int r2 = (a != b);
    int r3 = (a < b);
    int r4 = (b > a);
    int r5 = (a <= c);
    int r6 = (b >= a);
    
    int r7 = (a < b && b > 0);
    int r8 = (a > b || b > 0);
    int r9 = !(a > b);
    
    return 0;
}
