#include <stdio.h>

void primeFactors(int a, int m){
    int x, y;
    scanf("%d %d", &a, &m);
    if(a > 1) {
        for (x = 1; x <= m; x++){
            while (a % 2 == 0){
                printf("%d\n", 2);
                a = a / 2;
            }
            for (y = 3; y <= a; y = y + 2){
                while (a % y == 0){
                    printf("%d\n", y);
                    a = a / y;
                }
            }
        }
    }
    else
    {
        printf("ERROR");
    }
}

int main() {
    int a,m;
    primeFactors(a,m);
    return 0;
}