#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <pthread.h>

int *unsorted;
int *sorted;

int comparator(const void *int1, const void *int2);
void single_thread(int *unsorted, int length);
void *merge();

int main(int argc, char *argv[]) {
    FILE *in;
    char temp[40];
    int i;
    struct stat st;
    pthread_t tid = 2;
    pthread_attr_t attr;

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
    unsorted = (int *)malloc(st.st_size * 2);
    sorted = (int *)malloc(st.st_size * 2);

    while (fgets(temp, 40, in) != NULL) {
        unsorted[i] = atoi(temp);
        i++;
    }

    single_thread(unsorted, i);

    // Set up merge thread
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, merge, NULL);
    pthread_join(tid, NULL);
    printf("done\n");

    printf("SORTED\n");

    for (i = 0; i < 10; i++) {
        printf("%d\n", sorted[i]);
    }

    //free(unsorted);
    //free(sorted);
    return 0;
}

void single_thread(int *unsorted, int length) {
    int *unsorted_copy = (int *)malloc(length * sizeof(int));
    struct timespec start, finish;
    double elapsed;
    memcpy(unsorted_copy, unsorted, length * sizeof(int));

    clock_gettime(CLOCK_MONOTONIC, &start);
    qsort(unsorted_copy, length, sizeof(int), comparator);
    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Single threaded sorting takes %f sec\n", elapsed);
    //free(unsorted_copy);
}

int comparator(const void *int1, const void *int2) {
    return *(int *)int1 - *(int *)int2;
}

// Will be run in a seperate thread to merge two sorted lists
void *merge() {
    printf("Merging!!!\n");

    int start1 = 0;
    int end1 = 5;

    int start2 = 5;
    int end2 = 10;

    int i = 0; // Keep track of our spot in the sorted list

    while (start1 < end1 && start2 < end2) {
        if (unsorted[start1] < unsorted[start2]) {
            sorted[i] = unsorted[start1];
            start1++;
        } else {
            sorted[i] = unsorted[start2];
            start2++;
        }

        i++;
    }
    
    while (start1 < end1) {
        sorted[i] = unsorted[start1];
        start1++;
        i++;
    }

    while (start2 < end2) {
        sorted[i] = unsorted[start2];
        start2++;
        i++;
    }

    pthread_exit(0);
}
