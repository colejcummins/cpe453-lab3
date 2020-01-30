#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/stat.h>

int *unsorted;
int *sorted;

int comparator(const void *int1, const void *int2);
void single_thread(int length);
void multi_thread(int length);
void *sort_half(void *param);

int main(int argc, char *argv[]) {
    FILE *in, *out;
    char temp[40];
    int i, j;
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
    unsorted = (int *)malloc(st.st_size * 2);
    unsorted = (int *)malloc(st.st_size * 2);
    i = 0;

    while (fgets(temp, 40, in) != NULL) {
        unsorted[i] = atoi(temp);
        i++;
    }

    single_thread(i);
    multi_thread(i);

    out = fopen("sorted.txt", "w+");

    char str[20];
    for(int j = 0; j < i; j++) {
        sprintf(str, "%d\n");
        fputs(str, out);
        memeset(str, 0, 20);
    }

    fclose(out);

    //free(unsorted);
    return 0;
}

void multi_thread(int length) {
    int i;
    pthread_t first_half, second_half;

    int first_params[2] = {0, length/2};
    int second_params[2] = {length/2, (length - length/2)};

    pthread_create(&first_half, NULL, sort_half, first_params);
    pthread_create(&second_half, NULL, sort_half, second_params);

    pthread_join(first_half, NULL);
    pthread_join(second_half, NULL);

    for (i = 0; i < length; i++)
        printf("%d\n", unsorted[i]);
    printf("%d\n", i);
}

void *sort_half(void *param) {
    qsort(&unsorted[((int *)param)[0]], ((int *)param)[1], sizeof(int), comparator);
    pthread_exit(0);
}

void single_thread(int length) {
    int i;
    int *unsorted_copy = (int *)malloc(length * sizeof(int));
    struct timespec start, finish;
    double elapsed;
    memcpy(unsorted_copy, unsorted, length * sizeof(int));

    clock_gettime(CLOCK_MONOTONIC, &start);
    qsort(unsorted_copy, length, sizeof(int), comparator);
    clock_gettime(CLOCK_MONOTONIC, &finish);

    for (i = 0; i < length; i++)
        printf("%d\n", unsorted_copy[i]);

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Single threaded sorting takes %f sec\n", elapsed);
    //free(unsorted_copy);
}

int comparator(const void *int1, const void *int2) {
    return *(int *)int1 - *(int *)int2;
}
