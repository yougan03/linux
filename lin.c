#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>

#define MAX_STACK_SIZE 100

char *dir_stack[MAX_STACK_SIZE];
int stack_top = -1;

void pushd(const char *dir) {
    if (stack_top >= MAX_STACK_SIZE - 1) {
        fprintf(stderr, "Directory stack overflow\n");
        exit(EXIT_FAILURE);
    }

    char current_dir[PATH_MAX];
    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }

    if (chdir(dir) != 0) {
        perror("chdir");
        exit(EXIT_FAILURE);
    }

    dir_stack[++stack_top] = strdup(current_dir);

    printf("Directory stack:\n");
    for (int i = 0; i <= stack_top; i++) {
        printf("%d: %s\n", i, dir_stack[i]);
    }
}

void popd() {
    if (stack_top < 0) {
        fprintf(stderr, "Directory stack underflow\n");
        exit(EXIT_FAILURE);
    }

    char *dir = dir_stack[stack_top--];
    if (chdir(dir) != 0) {
        perror("chdir");
        exit(EXIT_FAILURE);
    }
    free(dir);

    printf("Directory stack:\n");
    for (int i = 0; i <= stack_top; i++) {
        printf("%d: %s\n", i, dir_stack[i]);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <pushd dir | popd>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "pushd") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Usage: %s pushd <dir>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        pushd(argv[2]);
    } else if (strcmp(argv[1], "popd") == 0) {
        if (argc != 2) {
            fprintf(stderr, "Usage: %s popd\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        popd();
    } else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    return 0;
}
