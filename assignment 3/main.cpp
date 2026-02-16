#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

void correlate(int ny, int nx, const float* data, float* result);

int main(int argc, char* argv[])
{
    if (argc < 3) {
        cout << "Usage: ./correlate_program ny nx\n";
        return 1;
    }

    int ny = atoi(argv[1]);
    int nx = atoi(argv[2]);

    float* data = new float[ny * nx];
    float* result = new float[ny * ny];

    srand(0);

    for (int i = 0; i < ny * nx; i++) {
        data[i] = (float)rand() / RAND_MAX;
    }

    correlate(ny, nx, data, result);

    cout << "Done computing correlations\n";

    delete[] data;
    delete[] result;

    return 0;
}
