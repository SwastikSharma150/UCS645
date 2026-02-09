#include <omp.h>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    const int grid_size = 400;
    const int time_steps = 400;

    vector<vector<double>> temperature(grid_size,
                                       vector<double>(grid_size, 0.0));
    vector<vector<double>> updated(grid_size,
                                   vector<double>(grid_size, 0.0));

    // Initial heat source at the center
    temperature[grid_size / 2][grid_size / 2] = 100.0;

    double t_start = omp_get_wtime();

    for (int step = 0; step < time_steps; step++) {

        double heat_sum = 0.0;

        #pragma omp parallel for reduction(+:heat_sum) schedule(static)
        for (int i = 1; i < grid_size - 1; i++) {
            for (int j = 1; j < grid_size - 1; j++) {

                updated[i][j] = 0.25 * (
                    temperature[i-1][j] +
                    temperature[i+1][j] +
                    temperature[i][j-1] +
                    temperature[i][j+1]
                );

                heat_sum += updated[i][j];
            }
        }

        temperature.swap(updated);
    }

    double t_end = omp_get_wtime();

    cout << "2D Heat Diffusion (OpenMP)\n";
    cout << "Simulation Time: " << t_end - t_start << " seconds\n";

    return 0;
}