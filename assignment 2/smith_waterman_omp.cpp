#include <omp.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Simple scoring function
inline int score(char a, char b) {
    return (a == b) ? 2 : -1;
}

int main() {
    string A = "ACACACTA";
    string B = "AGCACACA";

    const int gap_penalty = -1;

    int rows = A.size() + 1;
    int cols = B.size() + 1;

    vector<vector<int>> dp(rows, vector<int>(cols, 0));

    double t_start = omp_get_wtime();

    // Process matrix along anti-diagonals
    for (int diag = 2; diag <= rows + cols - 2; diag++) {

        #pragma omp parallel for schedule(static)
        for (int r = 1; r < rows; r++) {
            int c = diag - r;

            if (c >= 1 && c < cols) {
                int from_diag = dp[r-1][c-1] + score(A[r-1], B[c-1]);
                int from_up   = dp[r-1][c]   + gap_penalty;
                int from_left = dp[r][c-1]   + gap_penalty;

                int best = from_diag;
                if (from_up > best)   best = from_up;
                if (from_left > best) best = from_left;
                if (best < 0)         best = 0;

                dp[r][c] = best;
            }
        }
    }

    double t_end = omp_get_wtime();

    cout << "Smith-Waterman (OpenMP Wavefront)\n";
    cout << "Time taken: " << t_end - t_start << " seconds\n";

    return 0;
}