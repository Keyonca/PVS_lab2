#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 200000

int main() {
    int *array = malloc(SIZE * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < SIZE; i++) {
        array[i] = i + 1;
    }

    clock_t start = clock();

    long long sum = 0;
    for (int i = 0; i < SIZE; i++) {
        sum += array[i];
    }

    clock_t end = clock();
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Сумма массива: %lld\n", sum);
    printf("Время выполнения последовательной программы: %f секунд\n", time_taken);

    free(array);
    return 0;
}
