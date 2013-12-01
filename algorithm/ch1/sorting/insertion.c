#include <stdio.h>

/**
 * Insertion Sort: O(n^2)
 */
void swap(int *i, int *j) {
    int temp;
    temp = *i;
    *i = *j;
    *j = temp;
}

void insertion_sort(int *items, int n) {
    int i;
    int j;

    for (i = 1; i < n; ++i) {
        j = i;
        while ((j != 0) && (items[j] < items[j - 1])) {
            swap(&items[j], &items[j - 1]);
            --j;
        }
    }
}

void print_array(int *items, int n) {
    int i;
    for (i = 0; i < n; ++i) {
        printf("%d ", items[i]);
    }
    printf("\n\n");
}

int main(void) {
    int size = 5;
    int items[] = {10, 8 ,80, 5, -5};

    printf("Unsorted Array\n");
    print_array(items, size);

    insertion_sort(items, size);

    printf("Sorted Array\n");
    print_array(items, size);

    return 0;
}
/**
 * Analysis of insertion sort is harder because of the inner loop. We'll just
 * assume it goes around i times.
 */
