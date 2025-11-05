#include <stdio.h>

int main() {
int x;
scanf("%d", &x);
int y = 2;
int z = y * 3 + 1;
for(int i = 0; i < z; i++) {
    y = i + 3;
    if(i < 0)
        x--;
    else
        x++;
    printf("%d\n", x);
}
printf("%d %d %d", x, y, z);
}