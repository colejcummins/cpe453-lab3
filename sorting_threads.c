#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

int *unsorted;

int comparator(const void *int1, const void *int2);
void single_thread(int *unsorted, int length);

int main(int argc, char *argv[]) {
    FILE *in;
    char temp[40];
    int i;
    struct stat st;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    in = fopen(argv[1], "r");

    // Print error and exit if file cannot be opened
    if (in == NULL) {
        fprintf(stderr, "%s: ", argv[1]);
        perror("");
        exit(EXIT_FAILURE);
    }

    stat(argv[1], &st);
    unsorted = (int *)malloc(st.st_size);
    i = 0;

    while (fgets(temp, 40, in) != NULL) {
        unsorted[i] = atoi(temp);
        printf("%d\n", unsorted[i]);
        i++;
    }

    single_thread(unsorted, i - 1);
    free(unsorted);
    return 0;
}

void single_thread(int *unsorted, int length) {
    int i;
    int *unsorted_copy = (int *)malloc(length * sizeof(int));
    struct timespec start, finish;
    double elapsed;
    memcpy(unsorted_copy, unsorted, length * sizeof(int));

    clock_gettime(CLOCK_MONOTONIC, &start);
    qsort(unsorted_copy, length * sizeof(int), sizeof(int), comparator);
    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Single threaded sorting takes %f sec\n", elapsed);
    for (i = 0; i < length; i++);
        printf("%d\n", unsorted_copy[i]);
    free(unsorted_copy);
}

int comparator(const void *int1, const void *int2) {
    return (int *)int1 - (int *)int2;
}
