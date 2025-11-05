#include <iostream>

using namespace std;

int hit1;
int hit2;
int cache[16] = {0};
int total_time = 0;

int main() {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            for(int k = 0; k < 4; k++) {
                hit1 = 4 * i + k;
                hit2 = 4 * k + j;
                
                if(cache[hit1] == 1) {
                    total_time += 1;
                } else {
                    total_time += 21;
                    cache[hit1] = 1;
                }

                if(cache[hit2] == 1) {
                    total_time += 1;
                } else {
                    total_time += 21;
                    cache[hit2] = 1;
                }
            }
        }
    }

    cout << "Total time with cache: " << total_time << endl;
    return 0;
}