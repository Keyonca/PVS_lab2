#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    const char *env_size = getenv("ARRAY_SIZE");
    int size = env_size ? atoi(env_size) : 200000;

    int *array = malloc(size * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < size; i++) {
        array[i] = i + 1;
    }

    long long sum = 0;

    double start_time = omp_get_wtime();

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }

    double end_time = omp_get_wtime();

    printf("Сумма массива: %lld\n", sum);
    printf("Время выполнения параллельной программы: %f секунд с %d потоками\n",
           end_time - start_time, omp_get_max_threads());

    free(array);
    return 0;
}
