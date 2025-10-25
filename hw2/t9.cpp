#include <iostream>
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    int a[6] = {0};
    for (int i = 1; i <= 77; i++) {
        std::cout << "Cycle " << i << ":" << std::endl;
        a[1] = a[0];
        a[3] = a[2];
        a[5] = a[4];
        printArray(a, 6);
        a[0] = !a[5];
        a[2] = a[1];
        a[4] = a[3];
        printArray(a, 6);
    }
}