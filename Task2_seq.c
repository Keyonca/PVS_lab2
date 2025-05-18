#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

int main() {
    int N = 200000;
    int* arr = malloc(N * sizeof(int));

    srand(time(NULL));
    for (int i = 0; i < N; i++)
        arr[i] = rand();

    clock_t start = clock();
    quicksort(arr, 0, N - 1);
    clock_t end = clock();

    printf("Время выполнения быстрой сортировки в последовательной программе: %.2f секунд\n", (double)(end - start) / CLOCKS_PER_SEC);
    free(arr);
    return 0;
}
