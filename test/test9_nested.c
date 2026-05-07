/* Test 9: Nested Control Flow */
/* Tests: nested if, if inside while, complex branching */
int main() {
    int x = 15;
    int y = 0;
    
    if (x > 10) {
        if (x > 20) {
            y = 3;
        } else {
            y = 2;
        }
    } else {
        y = 1;
    }
    
    int i = 0;
    int count = 0;
    while (i < 5) {
        if (i > 2) {
            count = count + 1;
        }
        i = i + 1;
    }
    
    return 0;
}
