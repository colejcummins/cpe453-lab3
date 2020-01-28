#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    FILE *in;
    char temp[40];
    int *unsorted, i;
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
        i++;
    }

    free(unsorted);
    return 0;
}
