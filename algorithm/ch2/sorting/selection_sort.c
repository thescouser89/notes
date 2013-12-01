#include <stdio.h>
/**
 * Repeatedly identifies the smallest remaining unsorted element and puts it at
 * the end of the sorted portion of the array
 */
void selection_sort(int *s, int n) {
    int i;
    int j;
    int min;

    for (i = 0; i < n; ++i) {
        min = i;
        for (j = i + 1; j < n; ++j) {
            /* search for anything smaller than i */
            if (s[j] < s[min]) {
                min = j;
            }
        }
        /* swap the smallest item with i */
        swap(&s[i], &s[min]);
    }
}
/**
 * e.g
 * 64 25 12 22 11
 * 11 25 12 22 64
 * 11 12 25 22 64
 * 11 12 22 25 64
 * 11 12 22 25 64
 */

/**
 * Analysis
 *
 * S(n) = E^(n - 1)_i = 0 E^(n-1) _ (j = i + 1) = E^(n-1) _ i = 0 (n - i - 1)
 *      = n - 1 + n -2 + n - 3 + ... + 2 + 1
 *
 *      = n(n-1)/2
 */
