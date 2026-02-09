#include <omp.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>

using namespace std;

struct Particle {
    double x, y, z;
    double fx, fy, fz;
};

int main() {
    const int N = 1200;
    const double eps = 1.0;
    const double sig = 1.0;

    vector<Particle> particles(N);

    // Initialize positions and forces
    for (int i = 0; i < N; i++) {
        particles[i].x = drand48();
        particles[i].y = drand48();
        particles[i].z = drand48();
        particles[i].fx = particles[i].fy = particles[i].fz = 0.0;
    }

    double total_energy = 0.0;
    double t_start = omp_get_wtime();

    #pragma omp parallel
    {
        // Thread-local force buffers to reduce atomics
        vector<double> fx_local(N, 0.0);
        vector<double> fy_local(N, 0.0);
        vector<double> fz_local(N, 0.0);

        #pragma omp for schedule(guided) reduction(+:total_energy)
        for (int i = 0; i < N; i++) {
            for (int j = i + 1; j < N; j++) {

                double dx = particles[i].x - particles[j].x;
                double dy = particles[i].y - particles[j].y;
                double dz = particles[i].z - particles[j].z;

                double r2 = dx*dx + dy*dy + dz*dz;
                double inv_r2 = 1.0 / r2;
                double inv_r6 = inv_r2 * inv_r2 * inv_r2;

                double coeff = 24.0 * eps * inv_r2 * inv_r6 * (2.0*inv_r6 - 1.0);

                double fx = coeff * dx;
                double fy = coeff * dy;
                double fz = coeff * dz;

                fx_local[i] += fx;
                fy_local[i] += fy;
                fz_local[i] += fz;

                fx_local[j] -= fx;
                fy_local[j] -= fy;
                fz_local[j] -= fz;

                total_energy += 4.0 * eps * (inv_r6*inv_r6 - inv_r6);
            }
        }

        // Merge local forces into global arrays
        #pragma omp critical
        {
            for (int i = 0; i < N; i++) {
                particles[i].fx += fx_local[i];
                particles[i].fy += fy_local[i];
                particles[i].fz += fz_local[i];
            }
        }
    }

    double t_end = omp_get_wtime();

    cout << "Lennard-Jones Force Calculation\n";
    cout << "Total Potential Energy: " << total_energy << "\n";
    cout << "Execution Time: " << t_end - t_start << " seconds\n";

    return 0;
}