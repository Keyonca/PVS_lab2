#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void fill_arrays(double *a, double *b, int N) {
    for (int i = 0; i < N; i++) {
        a[i] = i + 1.0;
        b[i] = (i + 1.0) * 2.0;
    }
}

void sequential_operations(double *a, double *b, double *sum, double *diff, double *prod, double *div, int N) {
    for (int i = 0; i < N; i++) {
        sum[i]  = a[i] + b[i];
        diff[i] = a[i] - b[i];
        prod[i] = a[i] * b[i];
        div[i]  = a[i] / b[i];
    }
}

void parallel_operations(double *a, double *b, double *sum, double *diff, double *prod, double *div, int N) {
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

int main(int argc, char *argv[]) {
    int N = 200000;

    char *env_n = getenv("ARRAY_SIZE");
    if (env_n != NULL) {
        N = atoi(env_n);
    }

    if (argc > 1) {
        N = atoi(argv[1]);
    }

    double *a = malloc(N * sizeof(double));
    double *b = malloc(N * sizeof(double));
    double *sum  = malloc(N * sizeof(double));
    double *diff = malloc(N * sizeof(double));
    double *prod = malloc(N * sizeof(double));
    double *div  = malloc(N * sizeof(double));

    if (!a || !b || !sum || !diff || !prod || !div) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    fill_arrays(a, b, N);

    double t1 = get_time_sec();
    sequential_operations(a, b, sum, diff, prod, div, N);
    double t2 = get_time_sec();
    printf("Время выполнения последовательной программы: %f секунд\n", t2 - t1);

    double t_start = get_time_sec();
    parallel_operations(a, b, sum, diff, prod, div, N);
    double t_end = get_time_sec();
    int actual_threads = omp_get_max_threads();
    printf("Время выполнения параллельной программы с %d потоками: %f секунд\n", actual_threads, t_end - t_start);

    free(a); free(b); free(sum); free(diff); free(prod); free(div);
    return 0;
}
