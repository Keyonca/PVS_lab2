#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 200000

void fill_arrays(double *a, double *b) {
    for (int i = 0; i < N; i++) {
        a[i] = i + 1.0;
        b[i] = (i + 1.0) * 2.0;
    }
}

void sequential_operations(double *a, double *b, double *sum, double *diff, double *prod, double *div) {
    for (int i = 0; i < N; i++) {
        sum[i]  = a[i] + b[i];
        diff[i] = a[i] - b[i];
        prod[i] = a[i] * b[i];
        div[i]  = a[i] / b[i];
    }
}

void parallel_operations(double *a, double *b, double *sum, double *diff, double *prod, double *div, int threads) {
    omp_set_num_threads(threads);

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        sum[i]  = a[i] + b[i];
        diff[i] = a[i] - b[i];
        prod[i] = a[i] * b[i];
        div[i]  = a[i] / b[i];
    }
}

double get_time_sec() {
    return omp_get_wtime();
}

int main() {
    double *a = malloc(N * sizeof(double));
    double *b = malloc(N * sizeof(double));
    double *sum  = malloc(N * sizeof(double));
    double *diff = malloc(N * sizeof(double));
    double *prod = malloc(N * sizeof(double));
    double *div  = malloc(N * sizeof(double));

    fill_arrays(a, b);

    // Последовательный вариант
    double t1 = get_time_sec();
    sequential_operations(a, b, sum, diff, prod, div);
    double t2 = get_time_sec();
    printf("Sequential time: %f seconds\n", t2 - t1);

    int thread_counts[] = {2, 4, 8, 16};

    for (int i = 0; i < 4; i++) {
        int threads = thread_counts[i];
        double t_start = get_time_sec();
        parallel_operations(a, b, sum, diff, prod, div, threads);
        double t_end = get_time_sec();
        printf("Parallel time with %d threads: %f seconds\n", threads, t_end - t_start);
    }

    free(a); free(b); free(sum); free(diff); free(prod); free(div);
    return 0;
}
