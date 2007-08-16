#include <stdio.h>

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void sift(int a[], unsigned int start, unsigned int end) {
    unsigned int root = start;
    unsigned int child;

    while ((child = root * 2 + 1) <= end) {
        if (child + 1 <= end && a[child + 1] > a[child]) {
            child++;
        }

        if (a[child] > a[root]) {
            swap(&a[root], &a[child]);
            root = child;
        } else {
            return;
        }
    }
}

void heapsort(int arr[], size_t nmemb) {
    unsigned int start = nmemb / 2;

    while (start-- > 0) {
        sift(arr, start, nmemb - 1);
    }

    while (nmemb-- > 1) {
        swap(&arr[nmemb], &arr[0]);
        sift(arr, 0, nmemb - 1);
    }
}
void print_array(int *a, size_t size) {
    while (size-- > 0) {
        printf("%d%c", a[size], size == 0 ? '\n' : ' ');
    }
}
int main() {
    int a[] = { 23, 21, 1, 3, 5, 1234, 343, 51, 123, 663, 43, 90 };
    print_array(a, 12);   
    heapsort(a, 12);
    print_array(a, 12);   

    return 0;
}
