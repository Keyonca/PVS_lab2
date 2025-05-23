#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void fill_matrix(double *matrix, int rows, int cols) {
    for (int i = 0; i < rows * cols; i++)
        matrix[i] = (double)(rand() % 100 + 1);
}

void sequential_operations(double *A, double *B, double *Add, double *Sub, double *Mul, double *Div, int rows, int cols) {
    for (int i = 0; i < rows * cols; i++) {
        Add[i] = A[i] + B[i];
        Sub[i] = A[i] - B[i];
        Mul[i] = A[i] * B[i];
        Div[i] = (B[i] != 0) ? A[i] / B[i] : 0.0;
    }
}

void parallel_operations(double *A, double *B, double *Add, double *Sub, double *Mul, double *Div, int rows, int cols) {
    #pragma omp parallel for
    for (int i = 0; i < rows * cols; i++) {
        Add[i] = A[i] + B[i];
        Sub[i] = A[i] - B[i];
        Mul[i] = A[i] * B[i];
        Div[i] = (B[i] != 0) ? A[i] / B[i] : 0.0;
    }
}

double get_time_diff(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

int main() {
    int size = 500;
    char *env_size = getenv("MATRIX_SIZE");
    if (env_size != NULL) {
        size = atoi(env_size);
        if (size <= 0) {
            fprintf(stderr, "Неверный MATRIX_SIZE. Используется значение по умолчанию (500).\n");
            size = 500;
        }
    }

    int rows = size, cols = size;

    double *A = malloc(rows * cols * sizeof(double));
    double *B = malloc(rows * cols * sizeof(double));
    double *Add = malloc(rows * cols * sizeof(double));
    double *Sub = malloc(rows * cols * sizeof(double));
    double *Mul = malloc(rows * cols * sizeof(double));
    double *Div = malloc(rows * cols * sizeof(double));

    if (!A || !B || !Add || !Sub || !Mul || !Div) {
        fprintf(stderr, "Ошибка выделения памяти.\n");
        return 1;
    }

    srand(time(NULL));
    fill_matrix(A, rows, cols);
    fill_matrix(B, rows, cols);

    struct timespec start, end;

    printf("=== Последовательная версия ===\n");
    clock_gettime(CLOCK_MONOTONIC, &start);
    sequential_operations(A, B, Add, Sub, Mul, Div, rows, cols);
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("Время выполнения: %.6f сек\n", get_time_diff(start, end));

    printf("=== Параллельная версия (OMP_NUM_THREADS=%s) ===\n", getenv("OMP_NUM_THREADS"));
    clock_gettime(CLOCK_MONOTONIC, &start);
    parallel_operations(A, B, Add, Sub, Mul, Div, rows, cols);
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("Время выполнения: %.6f сек\n", get_time_diff(start, end));

    free(A); free(B); free(Add); free(Sub); free(Mul); free(Div);
    return 0;
}
