#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quicksort_parallel(int arr[], int low, int high, int depth) {
    if (low < high) {
        int pi = partition(arr, low, high);

        if (depth > 0) {
            #pragma omp parallel sections
            {
                #pragma omp section
                quicksort_parallel(arr, low, pi - 1, depth - 1);

                #pragma omp section
                quicksort_parallel(arr, pi + 1, high, depth - 1);
            }
        } else {
            quicksort_parallel(arr, low, pi - 1, 0);
            quicksort_parallel(arr, pi + 1, high, 0);
        }
    }
}

int main() {
    int N = 200000;
    int* arr = malloc(N * sizeof(int));

    srand(time(NULL));
    for (int i = 0; i < N; i++)
        arr[i] = rand();

    int num_threads = 16;

    double start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            num_threads = omp_get_num_threads();
            quicksort_parallel(arr, 0, N - 1, 4);
        }
    }

    double end = omp_get_wtime();

    printf("Время выполнения быстрой сортировки в последовательной программе: %.2f секунд\n", end - start);
    printf("Потоков использовано: %d\n", num_threads);

    free(arr);
    return 0;
}
